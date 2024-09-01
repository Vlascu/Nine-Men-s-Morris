#include "EnumStringMaker.h"
#include "JsonMessageHandler.h"

#include "StartingWindow.h"
#include "MoaraClient.h"
#include "UiBoard.h"

#include "Stylesheets.h"

#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QDebug>

#include <functional>

MoaraClient::MoaraClient(IClientSDKPtr sdk, QWidget* parent)
	: QMainWindow(parent)
	, m_board(nullptr)
	, m_movingPlayerLabel(new QLabel(""))
	, m_errorLabel(new QLabel(""))
	, m_undoButton(new QPushButton(this))
	, m_timerLabel(new QLabel("0"))
	, m_computerThinking(new QLabel(""))
	, m_sdk(sdk)
{
	m_sdk->SetListener(this);

	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MoaraClient::UpdateTimer);
	timer->start(250);

	ui.setupUi(this);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MoaraClient::~MoaraClient()
{
	delete m_board;
	delete m_errorLabel;
	delete m_movingPlayerLabel;
	delete m_undoButton;
	delete m_computerThinking;
}

void MoaraClient::OnError(const std::string& message)
{
	ShowErrorMessage(message);
}

void MoaraClient::OnInfo(const std::string& message)
{
	ShowMessage(QString::fromStdString(message));
}

void MoaraClient::OnSetupBoard(NodesInfo nodesInfo)
{
	QMetaObject::invokeMethod(this, "SetupWindow", Qt::QueuedConnection, Q_ARG(NodesInfo, nodesInfo));
}

void MoaraClient::OnPlayerLeft()
{
	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection, Q_ARG(QString, "A player has left the lobby"));
	QMetaObject::invokeMethod(this, "ShowStartingWindow", Qt::QueuedConnection);
}

void MoaraClient::OnUpdateTimer(int timeRemaining)
{
	m_timerLabel->setText(QString::number(timeRemaining));
}

void MoaraClient::OnWinningPlayer(const std::string& player)
{
	std::string message = "Game finished. The winning player is " + player;

	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(message)));
}

void MoaraClient::OnAddedPiece(int index, EPieceType pieceType)
{
	m_board->AddPiece(index, pieceType);
}

void MoaraClient::OnMovedPiece(int from, int to, EPieceType pieceType)
{
	m_board->ResetNodesHighlight();
	m_board->ResetComputerNodesHighlight();

	m_board->SwapPieces(from, to, pieceType);

	auto buttons = m_board->GetAllButtons();

	buttons[from]->Highlight(QPen(Qt::yellow, 5));
	buttons[to]->Highlight(QPen(Qt::green, 5));

	m_board->SetHighlightedNodeIndex(from);
	m_board->SetHighlightedNodeIndex(to);
}

void MoaraClient::OnRemovedPiece(int index)
{
	m_board->RemovePiece(index);

	m_board->ResetComputerNodesHighlight();
}

void MoaraClient::OnPlayerRemoved(const std::string& player)
{
	QString message = QString::fromStdString(player + " got removed.");
	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection, Q_ARG(QString, message));

	m_sdk->SetupBoard();
}

void MoaraClient::OnPlayerChanged(const std::string& player, bool isComputer)
{
	m_board->ResetNodesHighlight();

	SetLabels(EPieceTypeStringMaker::GetEnumFromString(player), isComputer);
}

void MoaraClient::OnStateChanged(EGameState state)
{
	std::string message;

	switch (state)
	{
	case EGameState::Placing:
		m_gameState = EGameState::Placing;
		message = "Keep placing.";
		break;
	case EGameState::Moving:
		m_gameState = EGameState::Moving;
		message = "Moving phase, select which node to move and where to move it.";
		break;
	case EGameState::Removing:
		m_gameState = EGameState::Removing;
		message = "Removing phase, select which node to remove.";
		OnHighlightRemove();
		break;
	case EGameState::Finished:
		m_sdk->GetWinnigPlayer();

		QMetaObject::invokeMethod(this, "ShowStartingWindow", Qt::QueuedConnection);
		return;
	default:
		return;
	}

	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(message)));
}

void MoaraClient::OnWindmill()
{
	QString message("You are now in windmill. Delete an oponent's piece.");
	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection, Q_ARG(QString, message));

	m_sdk->GetPossibleRemoves();
}

void MoaraClient::OnPossibleMoves(PiecesIndexes indexes)
{
	try
	{
		m_board->HighlightNodes(indexes);
	}
	catch (std::exception e)
	{
		QMetaObject::invokeMethod(this, "ShowErrorMessage", Qt::QueuedConnection, Q_ARG(std::string, e.what()));
	}
}

void MoaraClient::OnPossibleRemoves(PiecesIndexes indexes)
{
	try
	{
		m_board->HighlightNodes(indexes);
	}
	catch (std::exception e)
	{
		QMetaObject::invokeMethod(this, "ShowErrorMessage", Qt::QueuedConnection, Q_ARG(std::string, e.what()));
	}
}

void MoaraClient::OnServerDisconnect()
{
	QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection
		, Q_ARG(QString, QString::fromStdString("lost connection to server, please restart")));
	QMetaObject::invokeMethod(this, "ShowStartingWindow", Qt::QueuedConnection);
}

void MoaraClient::SetBoard(IUiBoard* board)
{
	m_board = board;
}

void MoaraClient::SetLabels(EPieceType playerType, bool isComputer)
{
	auto label = "Moving player: " + EPieceTypeStringMaker::GetStringFromEnum(playerType);

	m_movingPlayerLabel->setText(QString::fromStdString(label));

	QString message;

	if (m_gameState == EGameState::Removing)
	{
		message = isComputer ? "Computer deleted a piece" : "You can now delete an opponent's piece. Right-click to do so.";
		QMetaObject::invokeMethod(this, "ShowMessage", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(message.toStdString())));
	}
	else
	{
		if (isComputer)
		{
			QString playerType = m_movingPlayerLabel->text();
			playerType.append(" (computer) ");
			m_movingPlayerLabel->setText(playerType);
			m_computerThinking->setText(QString::fromStdString("Computer is thinking..."));
		}
		else {
			m_computerThinking->setText("");
		}
	}

	m_errorLabel->setText("");
}

void MoaraClient::UpdateBoard(NodesInfo nodeInfoList)
{
	m_board->UpdateBoard(nodeInfoList);
}

void MoaraClient::ShowMessage(const QString& message)
{
	auto messageBox = new QMessageBox(this);
	messageBox->setText(message);
	messageBox->setIcon(QMessageBox::Information);
	messageBox->setStyleSheet("background-color: rgb(255, 255, 255);");
	messageBox->show();
}

void MoaraClient::InitGraphics()
{
	setStyleSheet(windowBackground);

	if (!m_board)
	{
		ShowMessage("Board not set");
	}

	InitWidgets();

	InitLayouts();

	m_board->update();

	ConnectMethods();

}

void MoaraClient::ConnectMethods()
{
	if (UiBoard* board = dynamic_cast<UiBoard*>(m_board))
	{
		connect(board, &UiBoard::nodeClicked, this, &MoaraClient::OnNodeClicked);
		connect(board, &UiBoard::removeNodeClicked, this, &MoaraClient::OnRemoveNodeClicked);
		connect(board, &UiBoard::move, this, &MoaraClient::OnMove);
		connect(board, &UiBoard::highlightMove, this, &MoaraClient::OnHighlightMove);
		connect(m_undoButton, &QPushButton::clicked, this, &MoaraClient::OnUndo);
	}
}

void MoaraClient::InitLayouts()
{
	QHBoxLayout* buttonsLayout = new QHBoxLayout();

	buttonsLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
	buttonsLayout->addWidget(m_errorLabel);
	buttonsLayout->addWidget(m_undoButton, Qt::AlignHCenter);
	buttonsLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

	QHBoxLayout* labelLayout = new QHBoxLayout();

	labelLayout->addWidget(m_movingPlayerLabel);
	labelLayout->addWidget(m_computerThinking);
	labelLayout->addWidget(m_timerLabel);

	ui.vertical1->addLayout(labelLayout);
	ui.vertical1->addWidget(m_board);
	ui.vertical1->addLayout(buttonsLayout);
}

void MoaraClient::InitWidgets()
{
	CustomizeLabels(m_errorLabel);
	CustomizeLabels(m_movingPlayerLabel);
	CustomizeLabels(m_timerLabel);
	CustomizeLabels(m_computerThinking);

	m_board->setMinimumHeight(700);
	m_board->setMinimumWidth(700);

	m_errorLabel->setStyleSheet("QLabel { color : red; }");
	m_timerLabel->setStyleSheet("QLabel { color : red; }");

	m_sdk->GetActivePlayer();

	InitButtons();
}

void MoaraClient::CustomizeLabels(QLabel* label)
{
	QFont font;
	font.setPointSize(15);

	label->setAlignment(Qt::AlignHCenter);
	label->setFixedHeight(30);
	label->setFont(font);
}

void MoaraClient::InitButtons()
{
	m_undoButton->setStyleSheet(buttonStyle);
	m_undoButton->setText("Undo Move");
	m_undoButton->setFixedWidth(200);
	m_undoButton->setFixedHeight(50);
	m_undoButton->show();
}

void MoaraClient::ShowStartingWindow()
{
	auto startingWindow = new StartingWindow(m_sdk);
	startingWindow->show();
	this->close();
}

void MoaraClient::OnNodeClicked(uint8_t index)
{
	if (m_gameState == EGameState::Placing)
	{
		m_sdk->PlacePiece(index);
	}
}

void MoaraClient::OnRemoveNodeClicked(uint8_t index)
{
	m_sdk->RemovePiece(index);
}

void MoaraClient::OnMove(uint8_t fromIndex, uint8_t toIndex)
{
	m_sdk->MovePiece(fromIndex, toIndex);
}

void MoaraClient::OnHighlightMove(uint8_t index)
{
	try
	{
		m_sdk->GetPossibleMoves(index);
	}
	catch (std::exception e)
	{
		ShowErrorMessage(e.what());
	}
}

void MoaraClient::OnHighlightRemove()
{
	m_sdk->GetPossibleRemoves();
}

void MoaraClient::ShowErrorMessage(const std::string& error)
{
	m_errorLabel->setText(QString::fromStdString(error));

	QTimer::singleShot(1000, this, [this]() {
		m_errorLabel->clear();
		});
}

void MoaraClient::OnUndo()
{
	m_sdk->Undo();
}

void MoaraClient::UpdateTimer()
{
	m_sdk->GetTimer();
}

void MoaraClient::paintEvent(QPaintEvent* event)
{

}
