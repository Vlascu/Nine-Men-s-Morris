#include <gtest/gtest.h>
#include <memory>

#include "NormalBoard.h"
#include "Player.h"
#include "InvalidMoveException.h"
#include "BoardFactory.h"

class NormalBoardTest : public testing::Test
{
public:
	template <typename... Position>
	void AddPieces(EPlayerType player, Position... position)
	{
		(m_board->AddPiece(position, player), ...);
	}

protected:
	void SetUp()
	{
		m_board = std::make_unique<NormalBoard>();
		m_player1 = EPieceType::Black;
		m_player2 = EPieceType::White;
	}

	std::unique_ptr<IBoard> m_board;
	EPlayerType m_player1;
	EPlayerType m_player2;
};

TEST_F(NormalBoardTest, BoardInitialization)
{
	for (uint8_t index{ 0 }; index < SMALL_BOARD_COLUMN_SIZE * SMALL_BOARD_ROW_SIZE; index++)
		EXPECT_EQ(m_board->GetNodeType(index), EPieceType::None);
}

TEST_F(NormalBoardTest, AddPiece)
{
	m_board->AddPiece(0, m_player1);
	EXPECT_EQ(m_board->GetNodeType(0), EPieceType::Black);

	m_board->AddPiece(1, m_player2);
	EXPECT_EQ(m_board->GetNodeType(1), EPieceType::White);

	EXPECT_THROW(m_board->AddPiece(0, m_player1), std::exception);

	EXPECT_THROW(m_board->AddPiece(0, m_player2), std::exception);
}

TEST_F(NormalBoardTest, FullLine)
{
	AddPieces(m_player1, 0, 1, 2, 4);
	AddPieces(m_player2, 5, 13, 21, 6);

	std::vector<int> fullLineIndexes = { 0, 1, 2, 5, 13, 21};
	std::vector<int> nonFullLineIndexes = {7, 9, 17, 6, 4};

	for (const auto& v : fullLineIndexes)
		EXPECT_EQ(m_board->IsFullLine(v), true);

	for (const auto& v : nonFullLineIndexes)
		EXPECT_EQ(m_board->IsFullLine(v), false);
}

TEST_F(NormalBoardTest, FirstMovingIntoFullLine)
{
	AddPieces(m_player1, 2, 4, 11, 12);
	m_board->AddPiece(10, m_player2);

	m_board->MovePiece(11, 3, m_player1);

	EXPECT_EQ(m_board->GetBoardState(), EBoardState::Full_line);
}

TEST_F(NormalBoardTest, SecondMovingIntoFullLine)
{
	AddPieces(m_player1, 1, 6, 7);

	m_board->MovePiece(1, 0, m_player1);
	EXPECT_EQ(m_board->GetBoardState(), EBoardState::Full_line);
}

TEST_F(NormalBoardTest, RemovePiece)
{
	m_board->AddPiece(0, m_player1);
	m_board->AddPiece(1, m_player2);

	m_board->RemovePiece(0, m_player2);
	EXPECT_EQ(m_board->GetNodeType(0), EPieceType::None);
	m_board->RemovePiece(1, m_player1);
	EXPECT_EQ(m_board->GetNodeType(1), EPieceType::None);

	m_board->AddPiece(0, m_player1);
	m_board->AddPiece(1, m_player2);

	EXPECT_THROW(m_board->RemovePiece(0, m_player1), std::exception);
	EXPECT_THROW(m_board->RemovePiece(1, m_player2), std::exception);
	EXPECT_EQ(m_board->GetNodeType(0), EPieceType::Black);
	EXPECT_EQ(m_board->GetNodeType(1), EPieceType::White);

	EXPECT_THROW(m_board->RemovePiece(5, m_player1), std::exception);
	EXPECT_EQ(m_board->GetNodeType(5), EPieceType::None);
}

TEST_F(NormalBoardTest, RemovePieceSpecialRules)
{
	AddPieces(m_player1, 0, 1, 2);

	m_board->RemovePiece(2, m_player2);
	EXPECT_EQ(m_board->GetNodeType(2), EPieceType::None);

	AddPieces(m_player1, 2, 3);
	EXPECT_THROW(m_board->RemovePiece(1, m_player2); , std::exception);
}

TEST_F(NormalBoardTest, MovePiece)
{
	m_board->AddPiece(0, m_player1);
	m_board->AddPiece(1, m_player2);
	m_board->AddPiece(21, m_player1);
	m_board->AddPiece(9, m_player2);

	//correct move
	m_board->MovePiece(0, 7, m_player1);
	EXPECT_EQ(m_board->GetNodeType(0), EPieceType::None);

	EXPECT_EQ(m_board->GetNodeType(7), EPieceType::Black);

	//move on top of same player piece
	EXPECT_THROW(m_board->MovePiece(1, 9, m_player2), std::exception);

	//move on top of other player
	m_board->AddPiece(0, m_player1);
	EXPECT_THROW(m_board->MovePiece(1, 0, m_player2), std::exception);

	//move anywhere on the board
	m_board->MovePiece(9, 4, m_player2);
	EXPECT_EQ(m_board->GetNodeType(9), EPieceType::None);

	EXPECT_EQ(m_board->GetNodeType(4), EPieceType::White);

	//can't move anywhere
	m_board->AddPiece(2, m_player1);
	EXPECT_THROW(m_board->MovePiece(2, 20, m_player1), InvalidMoveException);

	//can't move another player's piece
	EXPECT_THROW(m_board->MovePiece(2, 3, m_player2), std::exception);
}

TEST_F(NormalBoardTest, LoadBoard)
{
	PieceTypeList players = { EPieceType::Black, EPieceType::White };

	std::ifstream file(BOARD_TEST_LOAD, std::ios_base::in);

	m_board->LoadBoard(players, file);

	auto nodes = m_board->GetAllNodes();

	EXPECT_EQ(nodes.at(0)->GetNeighbour(ENeighboursPosition::Top), nodes.at(1));
	EXPECT_EQ(nodes.at(0)->GetNeighbour(ENeighboursPosition::Right), nodes.at(2));

	EXPECT_EQ(nodes.at(1)->GetNeighbour(ENeighboursPosition::Bottom), nodes.at(0));
	EXPECT_EQ(nodes.at(1)->GetNeighbour(ENeighboursPosition::Left), nodes.at(2));

	EXPECT_EQ(nodes.at(2)->GetNeighbour(ENeighboursPosition::Right), nodes.at(1));
	EXPECT_EQ(nodes.at(2)->GetNeighbour(ENeighboursPosition::Left), nodes.at(0));

	EXPECT_EQ(nodes.at(0)->GetPieceType(), EPieceType::None);
	EXPECT_EQ(nodes.at(1)->GetPieceType(), EPieceType::Black);
	EXPECT_EQ(nodes.at(2)->GetPieceType(), EPieceType::None);

	EXPECT_EQ(m_board->GetPlayerPiecesToPlace(m_player1), 0);
	EXPECT_EQ(m_board->GetPlayerPiecesToPlace(m_player2), 1);
	EXPECT_EQ(m_board->GetPlayerPiecesOnTable(m_player1), 1);
	EXPECT_EQ(m_board->GetPlayerPiecesOnTable(m_player2), 0);
}

TEST_F(NormalBoardTest, FirstWindmillRule)
{
	AddPieces(m_player1, 5, 13, 18, 19, 22);

	m_board->MovePiece(22, 21, m_player1);

	EXPECT_EQ(m_board->GetBoardState(), EBoardState::Windmill);
}

TEST_F(NormalBoardTest, SecondWindmillRule)
{
	AddPieces(m_player1, 1, 9, 16, 19, 20);

	m_board->MovePiece(16, 17, m_player1);

	EXPECT_EQ(m_board->GetBoardState(), EBoardState::Windmill);
}

TEST_F(NormalBoardTest, ThirdWindmillRule)
{
	AddPieces(m_player1, 2, 6, 7, 9, 17);

	m_board->MovePiece(2, 1, m_player1);

	EXPECT_EQ(m_board->GetBoardState(), EBoardState::Windmill);
}

TEST_F(NormalBoardTest, ShouldNotBeWindmillRule)
{
	AddPieces(m_player1, 4, 5);
	AddPieces(m_player2, 12, 14, 21);

	m_board->MovePiece(21, 13, m_player2);

	EXPECT_NE(m_board->GetBoardState(), EBoardState::Windmill);
}

TEST_F(NormalBoardTest, ShouldNotBeWindmillRule2)
{
	AddPieces(m_player2, 3, 10, 12, 16, 17, 18);

	m_board->MovePiece(3, 11, m_player2);

	EXPECT_NE(m_board->GetBoardState(), EBoardState::Windmill);
}
TEST_F(NormalBoardTest, BlockedPlayer)
{
	AddPieces(m_player1, 1, 3, 5, 7);
	AddPieces(m_player2, 0, 2, 4, 6);

	EXPECT_EQ(m_board->GetPossibleMoves(m_player2).size(), 0);
}
TEST_F(NormalBoardTest, PossibleRemoves)
{
	AddPieces(m_player2, 1);

	EXPECT_EQ(m_board->GetPossibleRemoves(EPieceType::Black).size(), 1);
	EXPECT_EQ(m_board->GetPossibleRemoves(EPieceType::Black)[0], 1);
}
TEST_F(NormalBoardTest, BoardFactoryError)
{
	PieceTypeList players{ EPieceType::Black, EPieceType::White };

	EXPECT_THROW(BoardFactory::Create(EBoardType::None, players, NORMAL_BOARD_CONFIG, 9), std::invalid_argument);

	std::ifstream input(BOARD_TEST_LOAD);
	EXPECT_THROW(BoardFactory::Create(EBoardType::None, players, input), std::invalid_argument);

	input.close();
}