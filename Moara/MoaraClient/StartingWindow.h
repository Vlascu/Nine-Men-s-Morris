#pragma once

#include "EBoardSize.h"
#include "EBoardType.h"
#include "EGameState.h"
#include "EComputerLevel.h"
#include "MoaraClient.h"

#include "IClientSDK.h"

#include "IUiBoard.h"
#include "ui_StartingWindow.h"

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QMetaObject>

#include <vector>

using ButtonGroup = std::pair<QPushButton*, int>;
using UiButtons = std::vector<ButtonGroup>;

Q_DECLARE_METATYPE(NodesInfo)

class StartingWindow : public QMainWindow, public IClientSDKListener
{
	Q_OBJECT

public:
	StartingWindow(IClientSDKPtr sdk, QWidget* parent = nullptr);
	~StartingWindow();

public: // IClientSDKListener
	void OnGameStarted() override;
	void OnJoinedLobby(int lobbyId) override;
	void OnCreatedLobby(int lobbyId) override;
	void OnError(const std::string& message) override;
	void OnInfo(const std::string& message) override;
	void OnSetupBoard(NodesInfo nodesInfo) override;
	void OnChangedConfig(EBoardType type, EBoardSize size, EComputerLevel level) override;
	void OnPlayerLeft() override;
	void OnUpdateTimer(int timeRemaining) override {}
	void OnWinningPlayer(const std::string& player) override {}
	void OnAddedPiece(int index, EPieceType pieceType) override {}
	void OnMovedPiece(int from, int to, EPieceType pieceType) override {}
	void OnRemovedPiece(int index) override {}
	void OnPlayerRemoved(const std::string& player) override {}
	void OnPlayerChanged(const std::string& player, bool isComputer) override {}
	void OnStateChanged(EGameState state) override {}
	void OnWindmill() override {}
	void OnPossibleMoves(PiecesIndexes indexes) override {}
	void OnPossibleRemoves(PiecesIndexes indexes) override {}
	void OnServerDisconnect() override;

public slots:
	void OnStartGame();
	void OnTypeSelected();
	void OnSizeSelected();
	void OnPlayerSelected();
	void OnPaintButtons();
	void OnJoinLobby();
	void OnCreateLobby();
	
	void SetupWindow(NodesInfo nodesInfoList);
	void ShowMessage(const std::string& message);
	void SetLabelText(QLabel* label, const QString& text);
	void ChangeButtonColor(QPushButton* button);
private:
	void InitGraphics();

	void SetPvpConfig(int numberOfPlayers, int boardConfiguration);
	void SetAiPlayers(EComputerLevel level);
	void CheckAndSetAiPlayers(QRegularExpression regex, EComputerLevel level);

	int GetButtonGroup(QPushButton* button);

	template<typename... Slots>
	void ConnectButton(QPushButton* button, int group, Slots... uiSlots);
	void ConnectAllButtons();
private:
	Ui::StartingWindow ui;

	MoaraClient* m_boardWindow;

	EBoardType m_type;
	EBoardSize m_size;
	EComputerLevel m_level;
	GameConfigPtr m_config;

	UiButtons m_buttons;

	IClientSDKPtr m_sdk;
};

