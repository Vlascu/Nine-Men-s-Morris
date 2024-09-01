#include <gtest/gtest.h>
#include <memory>

#include "DiagonalsBoard.h"
#include "Player.h"
#include "BoardFactory.h"

class DiagonalsBoardTest : public testing::Test
{
public:
	template <typename... Positions>
	void AddPieces(std::shared_ptr<IPlayer> player, IBoardPtr& board, Positions... positions) {
		(board->AddPiece(positions, player->GetType()), ...);
	}

	void TestBoardNodes(IBoardPtr board)
	{
		auto nodes = board->GetAllNodes();

		EXPECT_EQ(nodes.at(0)->GetNeighbour(ENeighboursPosition::Right), nodes.at(1));
		EXPECT_EQ(nodes.at(0)->GetNeighbour(ENeighboursPosition::Bottom), nodes.at(7));
		EXPECT_EQ(nodes.at(0)->GetNeighbour(ENeighboursPosition::Bottom_Right), nodes.at(8));

		EXPECT_EQ(nodes.at(2)->GetNeighbour(ENeighboursPosition::Bottom_Left), nodes.at(10));

		EXPECT_EQ(nodes.at(6)->GetNeighbour(ENeighboursPosition::Top), nodes.at(7));
		EXPECT_EQ(nodes.at(6)->GetNeighbour(ENeighboursPosition::Top_Right), nodes.at(14));

		EXPECT_EQ(nodes.at(4)->GetNeighbour(ENeighboursPosition::Top_Left), nodes.at(12));
		EXPECT_EQ(nodes.at(4)->GetNeighbour(ENeighboursPosition::Left), nodes.at(5));

		EXPECT_EQ(nodes.at(0)->GetPieceType(), EPieceType::None);
		EXPECT_EQ(nodes.at(20)->GetPieceType(), EPieceType::None);

		EXPECT_EQ(board->GetPlayerPiecesToPlace(m_player1->GetType()), 9);
		EXPECT_EQ(board->GetPlayerPiecesToPlace(m_player2->GetType()), 9);
	}

protected:
	void SetUp()
	{
		PieceTypeList players = { EPieceType::Black, EPieceType::White };

		m_board = std::make_unique<DiagonalsBoard>();
		m_producedBoard = BoardFactory::Create(EBoardType::Diagonals, players, DIAGONALS_NORMAL_BOARD_CONFIG, 9);

		m_player1 = std::make_shared<Player>(EPieceType::Black, false);
		m_player2 = std::make_shared<Player>(EPieceType::White, false);
		m_player3 = std::make_shared<Player>(EPieceType::Red, false);
	}

	IBoardPtr m_board;
	IBoardPtr m_producedBoard;
	PlayerPtr m_player1;
	PlayerPtr m_player2;
	PlayerPtr m_player3;
};

TEST_F(DiagonalsBoardTest, FullLine)
{
	AddPieces(m_player1, m_board, 0, 1, 2, 3);
	AddPieces(m_player2, m_board, 5, 13, 21, 11);
	AddPieces(m_player3, m_board, 4, 12, 20, 19);

	EXPECT_EQ(m_board->IsFullLine(0), true);
	EXPECT_EQ(m_board->IsFullLine(1), true);
	EXPECT_EQ(m_board->IsFullLine(2), true);

	EXPECT_EQ(m_board->IsFullLine(5), true);
	EXPECT_EQ(m_board->IsFullLine(13), true);
	EXPECT_EQ(m_board->IsFullLine(21), true);

	EXPECT_EQ(m_board->IsFullLine(4), true);
	EXPECT_EQ(m_board->IsFullLine(12), true);
	EXPECT_EQ(m_board->IsFullLine(20), true);

	EXPECT_EQ(m_board->IsFullLine(3), false);
	EXPECT_EQ(m_board->IsFullLine(11), false);
	EXPECT_EQ(m_board->IsFullLine(19), false);

	EXPECT_EQ(m_board->IsFullLine(6), false);
	EXPECT_EQ(m_board->IsFullLine(14), false);
	EXPECT_EQ(m_board->IsFullLine(16), false);
	EXPECT_EQ(m_board->IsFullLine(17), false);
	EXPECT_EQ(m_board->IsFullLine(18), false);
	EXPECT_EQ(m_board->IsFullLine(22), false);
}

TEST_F(DiagonalsBoardTest, MovingIntoFullLine)
{
	AddPieces(m_player1, m_board, 2, 6, 12, 14, 18, 19, 23);

	m_board->AddPiece(11, m_player2->GetType());

	AddPieces(m_player3, m_board,  0, 1, 7);

	m_board->MovePiece(23, 22, m_player1->GetType());
	EXPECT_EQ(m_board->GetBoardState(), EBoardState::Full_line);
	m_board->RemovePiece(0, m_player1->GetType());
	EXPECT_EQ(m_board->GetBoardState(), EBoardState::None);

	m_board->MovePiece(11, 10, m_player2->GetType());
	EXPECT_EQ(m_board->GetBoardState(), EBoardState::None);

	m_board->MovePiece(12, 20, m_player1->GetType());
	EXPECT_EQ(m_board->GetBoardState(), EBoardState::Full_line);
	m_board->RemovePiece(1, m_player1->GetType());
	EXPECT_EQ(m_board->GetBoardState(), EBoardState::None);
}

TEST_F(DiagonalsBoardTest, CanBeFullLine)
{
	AddPieces(m_player1, m_producedBoard, 0, 2);

	EXPECT_TRUE(m_producedBoard->CanBeFullLine(1, EPieceType::Black));

	AddPieces(m_player1, m_producedBoard, 20, 4);

	EXPECT_TRUE(m_producedBoard->CanBeFullLine(12, EPieceType::Black));
}

TEST_F(DiagonalsBoardTest, LoadBoard)
{
	PieceTypeList players = { EPieceType::Black, EPieceType::White };

	std::ifstream file(DIAGONAL_BOARD_TEST_INIT, std::ios_base::in);

	m_board->LoadBoard(players, file);

	TestBoardNodes(m_board);

	std::ifstream newFile(DIAGONAL_BOARD_TEST_INIT, std::ios_base::in);
	auto board = BoardFactory::Create(EBoardType::Diagonals, players, newFile);

	TestBoardNodes(board);
}

TEST_F(DiagonalsBoardTest, SaveBoard)
{
	PieceTypeList players = { EPieceType::Black, EPieceType::White };
	{
		std::ifstream file(DIAGONAL_BOARD_TEST_INIT, std::ios_base::in);

		m_board->LoadBoard(players, file);

		AddPieces(m_player1, m_board, 0, 1, 2);

		file.close();
	}
	
	std::ofstream output("diagonal_board_save_test.txt");
	m_board->SaveBoard(output);

	output.close();

	std::ifstream file("diagonal_board_save_test.txt", std::ios_base::in);
	
	m_board->LoadBoard(players, file);

	auto nodes = m_board->GetAllNodes();

	EXPECT_EQ(nodes.at(0)->GetPieceType(), EPieceType::Black);
	EXPECT_EQ(nodes.at(1)->GetPieceType(), EPieceType::Black);
	EXPECT_EQ(nodes.at(2)->GetPieceType(), EPieceType::Black);
}