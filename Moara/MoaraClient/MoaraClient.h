#pragma once

#include "IUiBoard.h"
#include "ui_MoaraClient.h"

#include "IClientSDK.h"

#include <QtWidgets/QMainWindow>
#include <QPaintEvent>
#include <QMessageBox>

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

class MoaraClient : public QMainWindow, public IClientSDKListener
{
    Q_OBJECT

public:
    MoaraClient(IClientSDKPtr sdk, QWidget *parent = nullptr);

    ~MoaraClient();

    // IClientSDKListener
	void OnGameStarted() override {}
	void OnJoinedLobby(int lobbyId) override {}
	void OnCreatedLobby(int lobbyId) override {}
	void OnError(const std::string& message) override;
	void OnInfo(const std::string& message) override;
	void OnSetupBoard(NodesInfo nodesInfo) override;
	void OnChangedConfig(EBoardType type, EBoardSize size, EComputerLevel level) override {}
	void OnPlayerLeft() override;
	void OnUpdateTimer(int timeRemaining) override;
	void OnWinningPlayer(const std::string& player) override;
	void OnAddedPiece(int index, EPieceType pieceType) override;
	void OnMovedPiece(int from, int to, EPieceType pieceType) override;
	void OnRemovedPiece(int index) override;
	void OnPlayerRemoved(const std::string& player) override;
	void OnPlayerChanged(const std::string& player, bool isComputer) override;
	void OnStateChanged(EGameState state) override;
	void OnWindmill() override;
	void OnPossibleMoves(PiecesIndexes indexes) override;
	void OnPossibleRemoves(PiecesIndexes indexes) override;
	void OnServerDisconnect() override;

    // own methods
    void SetBoard(IUiBoard* board);

	void InitGraphics();

private:
	void InitLayouts();
	void InitWidgets();
    void InitButtons();

    void SetLabels(EPieceType playerType, bool isComputer);
    void CustomizeLabels(QLabel* label);

	void ConnectMethods();

public slots:
    void UpdateBoard(NodesInfo nodeInfoList);

	void ShowMessage(const QString& message);
	void ShowErrorMessage(const std::string& error);
    void ShowStartingWindow();

	void OnNodeClicked(uint8_t index);
    void OnRemoveNodeClicked(uint8_t index);
    void OnMove(uint8_t fromIndex, uint8_t toIndex);
	void OnUndo();

    void OnHighlightMove(uint8_t index);
    void OnHighlightRemove();

private slots:
	void UpdateTimer();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::MoaraClientClass ui;
    IUiBoard* m_board;

    QLabel* m_errorLabel;
    QLabel* m_timerLabel;
    QLabel* m_movingPlayerLabel;
    QLabel* m_computerThinking;
    QPushButton* m_undoButton;

    IClientSDKPtr m_sdk;
    
    EGameState m_gameState;
};
