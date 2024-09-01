
#include "JsonMessageHandler.h"
#include "EnumStringMaker.h"

#include "StartingWindow.h"
#include "MoaraClient.h"

#include "Stylesheets.h"

#include <QRegularExpression>
#include <QMessageBox>

#include <ranges>
#include <algorithm>

StartingWindow::StartingWindow(IClientSDKPtr sdk, QWidget* parent)
	: QMainWindow(parent)
	, m_type(EBoardType::Normal)
	, m_size(EBoardSize::Normal)
	, m_level(EComputerLevel::Easy)
	, m_config()
	, m_sdk(sdk)
{
	qRegisterMetaType<NodesInfo>("NodesInfo");
	m_sdk->SetListener(this);
	ui.setupUi(this);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_config = std::make_shared<GameConfig>();

	InitGraphics();
	ConnectAllButtons();
}

StartingWindow::~StartingWindow()
{
}

void StartingWindow::ConnectAllButtons()
{
	ConnectButton(ui.normalSizeButton, 0, &StartingWindow::OnSizeSelected, &StartingWindow::OnPaintButtons);
	ConnectButton(ui.bigSizeButton, 0, &StartingWindow::OnSizeSelected, &StartingWindow::OnPaintButtons);
	ConnectButton(ui.normalTypeButton, 1, &StartingWindow::OnTypeSelected, &StartingWindow::OnPaintButtons);
	ConnectButton(ui.diagonalTypeButton, 1, &StartingWindow::OnTypeSelected, &StartingWindow::OnPaintButtons);
	ConnectButton(ui.startGameButton, -1, &StartingWindow::OnStartGame);
	ConnectButton(ui.easyGame, 2, &StartingWindow::OnPlayerSelected, &StartingWindow::OnPaintButtons);
	ConnectButton(ui.mediumGame, 2, &StartingWindow::OnPlayerSelected, &StartingWindow::OnPaintButtons);
	ConnectButton(ui.hardGame, 2, &StartingWindow::OnPlayerSelected, &StartingWindow::OnPaintButtons);
	ConnectButton(ui.pvpButton, 2, &StartingWindow::OnPlayerSelected, &StartingWindow::OnPaintButtons);
	ConnectButton(ui.joinLobby, -1, &StartingWindow::OnJoinLobby);
	ConnectButton(ui.createLobby, -1, &StartingWindow::OnCreateLobby);
}

void StartingWindow::InitGraphics()
{
	setStyleSheet(windowBackground);

	this->setMinimumHeight(700);
	this->setMinimumWidth(700);

	ui.normalSizeButton->setStyleSheet(pushedButtonStyle);
	ui.normalTypeButton->setStyleSheet(pushedButtonStyle);
	ui.easyGame->setStyleSheet(pushedButtonStyle);

	ui.numberOfAI->setText(QString::fromStdString("1"));

	ui.levelLabel->setText(QString::fromStdString("AI Level: Easy"));
	ui.typeLabel->setText(QString::fromStdString("Board Type: Normal"));
	ui.sizeLabel->setText(QString::fromStdString("Board Size: Normal"));
	ui.playersLabel->setText(QString::fromStdString("AI Count: 1"));

}

template<typename... Slots>
void StartingWindow::ConnectButton(QPushButton* button, int group, Slots... uiSlots)
{
	bool found = false;

	for (auto storedButton : m_buttons)
	{
		if (storedButton.first == button)
		{
			found = true;
			break;
		}
	}

	if(!found)
		m_buttons.emplace_back(button, group);

	(connect(button, &QPushButton::clicked, this, uiSlots), ...);
}

void StartingWindow::OnTypeSelected()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());

	if (button) {
		QString buttonText = button->text();

		if (buttonText == "Normal")
		{
			m_type = EBoardType::Normal;
			ui.typeLabel->setText(QString::fromStdString("Board Type: Normal"));
		}
		else
		{
			m_type = EBoardType::Diagonals;
			ui.typeLabel->setText(QString::fromStdString("Board Type: Diagonals"));
		}
	}

	m_sdk->ChangeConfig(static_cast<int>(m_size), static_cast<int>(m_type), m_config->GetComputerLevel());
}

void StartingWindow::OnSizeSelected()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());

	if (button) {
		QString buttonText = button->text();

		if (buttonText == "Normal")
		{
			m_size = EBoardSize::Normal;
			ui.sizeLabel->setText(QString::fromStdString("Board Size: Normal"));
		}
		else
		{
			m_size = EBoardSize::Big;
			ui.sizeLabel->setText(QString::fromStdString("Board Size: Big"));
		}
	}
	m_sdk->ChangeConfig(static_cast<int>(m_size), static_cast<int>(m_type), m_config->GetComputerLevel());
}

void StartingWindow::OnPlayerSelected()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());

	if (button)
	{
		QString buttonText = button->text();
		QRegularExpression regex("^\\d+$");

		if (buttonText == "Easy Game")
		{
			CheckAndSetAiPlayers(regex, EComputerLevel::Easy);

			ui.levelLabel->setText(QString::fromStdString("AI Level: Easy"));

			m_level = EComputerLevel::Easy;
		}
		else if (buttonText == "Medium Game")
		{
			CheckAndSetAiPlayers(regex, EComputerLevel::Medium);

			ui.levelLabel->setText(QString::fromStdString("AI Level: Medium"));

			m_level = EComputerLevel::Medium;
		}
		else if (buttonText == "Hard Game")
		{
			CheckAndSetAiPlayers(regex, EComputerLevel::Hard);

			ui.levelLabel->setText(QString::fromStdString("AI Level: Hard"));
			m_level = EComputerLevel::Hard;
		}
		else if (buttonText == "PvP")
		{

			if (m_size == EBoardSize::Normal)
			{
				SetPvpConfig(2, 0);
			}
			else if (m_size == EBoardSize::Big)
			{
				SetPvpConfig(3, 1);
			}
			else
			{
				ShowMessage("First select board size");
				return;
			}

			ui.levelLabel->setText(QString::fromStdString("PvP"));
		}

		m_sdk->ChangeConfig(static_cast<int>(m_size), static_cast<int>(m_type), m_level);
	}
}

void StartingWindow::OnPaintButtons()
{
	auto senderButton = qobject_cast<QPushButton*>(sender());
	if (senderButton)
	{
		for (auto button : m_buttons)
		{
			if (button.first == senderButton) 
			{
				button.first->setStyleSheet(pushedButtonStyle);
			}
			else if (button.first != senderButton && button.second == GetButtonGroup(senderButton))
			{
				button.first->setStyleSheet(buttonStyle);
			}
			button.first->update();
		}
	}
}

void StartingWindow::OnJoinLobby()
{
	QRegularExpression regex("^\\d+$");

	QString lobbyIdString = ui.lobbyIdInput->text();

	if (regex.match(lobbyIdString).hasMatch())
		m_sdk->JoinLobby(lobbyIdString.toInt());
	else
		ShowMessage("Insert a number for lobby id");

}

void StartingWindow::OnCreateLobby()
{
	m_sdk->CreateLobby();
}

void StartingWindow::SetupWindow(NodesInfo nodeInfoList)
{
	auto boardWindow = new MoaraClient(m_sdk);
	auto board = IUiBoard::Produce(m_type, m_size, nodeInfoList);

	boardWindow->SetBoard(board);
	boardWindow->InitGraphics();
	boardWindow->show();

	this->close();
}

void StartingWindow::SetPvpConfig(int numberOfPlayers, int boardConfiguration)
{
	m_config->SetNumberOfPlayers(numberOfPlayers);
	m_config->SetPlayersConfig(humanBoardConfiguration, boardConfiguration);
}

void StartingWindow::CheckAndSetAiPlayers(QRegularExpression regex, EComputerLevel level)
{
	if (regex.match(ui.numberOfAI->text()).hasMatch())
	{
		SetAiPlayers(level);

		ui.playersLabel->setText(QString::fromStdString("AI Count: ") + ui.numberOfAI->text());
	}
	else
	{
		ShowMessage("Please insert a number of AI players");
	}
}

int StartingWindow::GetButtonGroup(QPushButton* button)
{
	for (auto [storedButton, group] : m_buttons)
	{
		if (button == storedButton)
			return group;
	}
	return -1;
}

void StartingWindow::ChangeButtonColor(QPushButton* button)
{
	auto buttonGroup = GetButtonGroup(button);

	button->setStyleSheet(pushedButtonStyle);

	for (auto& buttonPair : m_buttons)
	{
		if (buttonPair.second == buttonGroup && buttonPair.first != button)
			buttonPair.first->setStyleSheet(buttonStyle);
	}
}

void StartingWindow::ShowMessage(const std::string& message)
{
	auto messageBox = new QMessageBox(this);
	messageBox->setText(QString::fromStdString(message));
	messageBox->setIcon(QMessageBox::Information);
	messageBox->setStyleSheet("background-color: rgb(255, 255, 255);");
	messageBox->show();
}

void StartingWindow::SetLabelText(QLabel* label, const QString& text)
{
	if (text[0] == 'J')
	{
		auto sp = ui.startGameButton->sizePolicy();
		sp.setRetainSizeWhenHidden(true);

		ui.startGameButton->setSizePolicy(sp);
		ui.startGameButton->hide();
	}

	label->setText(text);
}

void StartingWindow::SetAiPlayers(EComputerLevel level)
{
	bool ok;
	int numberOfAI = ui.numberOfAI->text().toInt(&ok);

	if (m_size == EBoardSize::Normal)
	{
		m_config->SetNumberOfPlayers(2);

		if (numberOfAI < 1 || numberOfAI > 2)
		{
			ShowMessage("The number of AI players shouldn't pass 2 or be smaller than 1");
		}
		else
		{
			if(ok)
				m_config->SetPlayersConfig(smallAIBoardConfiguration, numberOfAI - 1);

		}
	}
	else if (m_size == EBoardSize::Big)
	{
		m_config->SetNumberOfPlayers(3);

		if (numberOfAI < 1 || numberOfAI > 3)
		{
			ShowMessage("The number of AI players shouldn't pass 3 or be smaller than 1");
		}
		else
		{
			if(ok)
				m_config->SetPlayersConfig(bigAIBoardConfiguration, numberOfAI - 1);
		}
	}

	m_config->SetComputerLevel(level);
}

void StartingWindow::OnGameStarted()
{
	m_sdk->SetupBoard();
}

void StartingWindow::OnJoinedLobby(int lobbyId)
{
	QMetaObject::invokeMethod(this, "SetLabelText", Qt::QueuedConnection,
		Q_ARG(QLabel*, ui.createdLobbyIdLabel),
		Q_ARG(QString, QString::fromStdString("Joined lobby id: " + std::to_string(lobbyId))));
}

void StartingWindow::OnCreatedLobby(int lobbyId)
{
	QMetaObject::invokeMethod(this, "SetLabelText", Qt::QueuedConnection,
		Q_ARG(QLabel*, ui.createdLobbyIdLabel),
		Q_ARG(QString, QString::fromStdString("Created lobby id: " + std::to_string(lobbyId))));
}

void StartingWindow::OnError(const std::string& message)
{
	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection
		, Q_ARG(std::string, message));
}

void StartingWindow::OnInfo(const std::string& message)
{

}

void StartingWindow::OnSetupBoard(NodesInfo nodesInfo)
{
	QMetaObject::invokeMethod(this, "SetupWindow", Qt::QueuedConnection, Q_ARG(NodesInfo, nodesInfo));
}

void StartingWindow::OnChangedConfig(EBoardType type, EBoardSize size, EComputerLevel level)
{
	switch (static_cast<EBoardSize>(size))
	{
	case EBoardSize::Normal:
		m_size = EBoardSize::Normal;

		QMetaObject::invokeMethod(this, "ChangeButtonColor", Qt::QueuedConnection, Q_ARG(QPushButton*, ui.normalSizeButton));
		ui.sizeLabel->setText(QString::fromStdString("Board Size: Normal"));
		break;
	case EBoardSize::Big:
		m_size = EBoardSize::Big;

		QMetaObject::invokeMethod(this, "ChangeButtonColor", Qt::QueuedConnection, Q_ARG(QPushButton*, ui.bigSizeButton));
		ui.sizeLabel->setText(QString::fromStdString("Board Size: Big"));
		break;
	default:
		break;
	}

	switch (static_cast<EBoardType>(type))
	{
	case EBoardType::Normal:
		m_type = EBoardType::Normal;

		QMetaObject::invokeMethod(this, "ChangeButtonColor", Qt::QueuedConnection, Q_ARG(QPushButton*, ui.normalTypeButton));
		ui.typeLabel->setText(QString::fromStdString("Board Type: Normal"));
		break;
	case EBoardType::Diagonals:
		m_type = EBoardType::Diagonals;

		QMetaObject::invokeMethod(this, "ChangeButtonColor", Qt::QueuedConnection, Q_ARG(QPushButton*, ui.diagonalTypeButton));
		ui.typeLabel->setText(QString::fromStdString("Board Type: Diagonals"));
		break;
	default:
		break;
	}

	switch (level)
	{
	case EComputerLevel::Easy:
		QMetaObject::invokeMethod(this, "ChangeButtonColor", Qt::QueuedConnection, Q_ARG(QPushButton*, ui.easyGame));
		ui.levelLabel->setText(QString::fromStdString("AI Level: Easy"));
		break;
	case EComputerLevel::Medium:
		QMetaObject::invokeMethod(this, "ChangeButtonColor", Qt::QueuedConnection, Q_ARG(QPushButton*, ui.mediumGame));
		ui.levelLabel->setText(QString::fromStdString("AI Level: Medium"));
		break;
	case EComputerLevel::Hard:
		QMetaObject::invokeMethod(this, "ChangeButtonColor", Qt::QueuedConnection, Q_ARG(QPushButton*, ui.hardGame));
		ui.levelLabel->setText(QString::fromStdString("AI Level: Hard"));
		break;
	case EComputerLevel::None:
		QMetaObject::invokeMethod(this, "ChangeButtonColor", Qt::QueuedConnection, Q_ARG(QPushButton*, ui.pvpButton));
		ui.levelLabel->setText(QString::fromStdString("PvP"));
		break;
	default:
		break;
	}
}

void StartingWindow::OnPlayerLeft()
{
	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection, Q_ARG(std::string, "A player has left the lobby"));
}

void StartingWindow::OnServerDisconnect()
{
	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection
		, Q_ARG(std::string, "lost connection to server, please restart"));
}

void StartingWindow::OnStartGame()
{
	m_config->SetBoardConfig(m_type, m_size);

	m_sdk->StartGame(m_config);
}

