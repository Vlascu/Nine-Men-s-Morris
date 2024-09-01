#include <gtest/gtest.h>
#include <memory>

#include "NormalNode.h"
#include "DiagonalsNode.h"
#include "InvalidNeighbourException.h"


class NodeTest : public testing::Test
{
protected:
	void SetUp()
	{
		for (int i{ 0 }; i < 10; ++i)
		{
			normalNodes.emplace_back(std::make_shared<NormalNode>());
			diagonalNodes.emplace_back(std::make_shared<DiagonalsNode>());
		}
	}

	std::vector<std::shared_ptr<INode>> normalNodes;
	std::vector<std::shared_ptr<INode>> diagonalNodes;
};

TEST_F(NodeTest, NodeInitialization)
{
	for (int i{ 0 }; i < 10; ++i)
		EXPECT_EQ(normalNodes[i]->GetPieceType(), EPieceType::None);

	auto neighbours = normalNodes[1]->GetNeighbours();
	for (const auto& neighbour : neighbours)
		EXPECT_EQ(neighbour, nullptr);

	//diagonals
	for (int i{ 0 }; i < 10; ++i)
		EXPECT_EQ(diagonalNodes[i]->GetPieceType(), EPieceType::None);

	neighbours = diagonalNodes[1]->GetNeighbours();
	for (const auto& neighbour : neighbours)
		EXPECT_EQ(neighbour, nullptr);
}

TEST_F(NodeTest, GettersSetters)
{
	normalNodes[0]->SetPiece(EPieceType::Black);
	EXPECT_EQ(normalNodes[0]->GetPieceType(), EPieceType::Black);

	normalNodes[0]->SetPiece(EPieceType::White);
	EXPECT_EQ(normalNodes[0]->GetPieceType(), EPieceType::White);

	normalNodes[0]->SetPiece(EPieceType::White);
	EXPECT_EQ(normalNodes[0]->GetPieceType(), EPieceType::White);

	normalNodes[0]->SetNeighbour(normalNodes[1].get(), ENeighboursPosition::Right);
	EXPECT_EQ(normalNodes[0]->GetNeighbour(ENeighboursPosition::Right), normalNodes[1].get());
	EXPECT_NE(normalNodes[1]->GetNeighbour(ENeighboursPosition::Left), normalNodes[0].get());

	std::vector<INode*> neighbours;
	neighbours.emplace_back(normalNodes[1].get());
	neighbours.emplace_back(normalNodes[2].get());
	neighbours.emplace_back(normalNodes[3].get());
	neighbours.emplace_back(normalNodes[4].get());
	normalNodes[0]->SetNeighbours(neighbours);
	EXPECT_EQ(normalNodes[0]->GetNeighbour(ENeighboursPosition::Top), normalNodes[1].get());
	EXPECT_EQ(normalNodes[0]->GetNeighbour(ENeighboursPosition::Right), normalNodes[2].get());
	EXPECT_EQ(normalNodes[0]->GetNeighbour(ENeighboursPosition::Bottom), normalNodes[3].get());
	EXPECT_EQ(normalNodes[0]->GetNeighbour(ENeighboursPosition::Left), normalNodes[4].get());
	EXPECT_EQ(normalNodes[0]->GetNeighbours(), neighbours);

	//diagonals
	diagonalNodes[0]->SetPiece(EPieceType::Black);
	EXPECT_EQ(diagonalNodes[0]->GetPieceType(), EPieceType::Black);

	diagonalNodes[0]->SetPiece(EPieceType::White);
	EXPECT_EQ(diagonalNodes[0]->GetPieceType(), EPieceType::White);

	diagonalNodes[0]->SetPiece(EPieceType::White);
	EXPECT_EQ(diagonalNodes[0]->GetPieceType(), EPieceType::White);

	diagonalNodes[0]->SetNeighbour(diagonalNodes[1].get(), ENeighboursPosition::Top_Right);
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Top_Right), diagonalNodes[1].get());
	EXPECT_NE(diagonalNodes[1]->GetNeighbour(ENeighboursPosition::Bottom_Left), diagonalNodes[0].get());

	neighbours.clear();
	neighbours.emplace_back(diagonalNodes[1].get());
	neighbours.emplace_back(diagonalNodes[2].get());
	neighbours.emplace_back(diagonalNodes[3].get());
	neighbours.emplace_back(diagonalNodes[4].get());
	neighbours.emplace_back(diagonalNodes[5].get());
	neighbours.emplace_back(diagonalNodes[6].get());
	neighbours.emplace_back(diagonalNodes[7].get());
	neighbours.emplace_back(diagonalNodes[8].get());
	diagonalNodes[0]->SetNeighbours(neighbours);
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Top), diagonalNodes[1].get());
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Top_Right), diagonalNodes[2].get());
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Right), diagonalNodes[3].get());
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Bottom_Right), diagonalNodes[4].get());
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Bottom), diagonalNodes[5].get());
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Bottom_Left), diagonalNodes[6].get());
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Left), diagonalNodes[7].get());
	EXPECT_EQ(diagonalNodes[0]->GetNeighbour(ENeighboursPosition::Top_Left), diagonalNodes[8].get());
	EXPECT_EQ(diagonalNodes[0]->GetNeighbours(), neighbours);
}

TEST_F(NodeTest, NodeExceptions)
{
	ENeighboursPosition invalidPosition = static_cast<ENeighboursPosition>(999);
	EXPECT_THROW(normalNodes[0]->SetNeighbour(normalNodes[1].get(), invalidPosition), InvalidNeighbourException);
	EXPECT_THROW(diagonalNodes[0]->SetNeighbour(diagonalNodes[1].get(), invalidPosition), InvalidNeighbourException);

	EXPECT_THROW(normalNodes[0]->GetNeighbour(invalidPosition), InvalidNeighbourException);
	EXPECT_THROW(diagonalNodes[0]->GetNeighbour(invalidPosition), InvalidNeighbourException);


	std::vector<INode*> neighbours = {normalNodes[0].get()};

	EXPECT_THROW(normalNodes[0]->SetNeighbours(neighbours), InvalidNeighbourException);
	EXPECT_THROW(diagonalNodes[0]->SetNeighbours(neighbours), InvalidNeighbourException);

	for(int i = 0; i < 100; ++i)
		neighbours.emplace_back(diagonalNodes[0].get());

	EXPECT_THROW(normalNodes[0]->SetNeighbours(neighbours), InvalidNeighbourException);
	EXPECT_THROW(diagonalNodes[0]->SetNeighbours(neighbours), InvalidNeighbourException);
}

TEST_F(NodeTest, NodeCloning)
{
	std::vector<INode*> neighbours;
	neighbours.emplace_back(normalNodes[1].get());
	neighbours.emplace_back(normalNodes[2].get());
	neighbours.emplace_back(normalNodes[3].get());
	neighbours.emplace_back(normalNodes[4].get());
	normalNodes[0]->SetNeighbours(neighbours);
	normalNodes[0]->SetPiece(EPieceType::Black);
	normalNodes[0]->SetIndex(0);

	auto clone = normalNodes[0]->Clone();
	EXPECT_EQ(clone->GetIndex(), 0);
	EXPECT_EQ(clone->GetPieceType(), EPieceType::Black);
	EXPECT_EQ(clone->GetNeighbours(), neighbours);

	//diagonals
	neighbours.clear();
	neighbours.emplace_back(diagonalNodes[0].get());
	neighbours.emplace_back(diagonalNodes[2].get());
	neighbours.emplace_back(diagonalNodes[3].get());
	neighbours.emplace_back(diagonalNodes[4].get());
	neighbours.emplace_back(diagonalNodes[5].get());
	neighbours.emplace_back(diagonalNodes[6].get());
	neighbours.emplace_back(diagonalNodes[7].get());
	neighbours.emplace_back(diagonalNodes[8].get());
	diagonalNodes[1]->SetNeighbours(neighbours);
	diagonalNodes[1]->SetPiece(EPieceType::White);
	diagonalNodes[1]->SetIndex(1);

	clone = diagonalNodes[1]->Clone();
	EXPECT_EQ(clone->GetIndex(), 1);
	EXPECT_EQ(clone->GetPieceType(), EPieceType::White);
	EXPECT_EQ(clone->GetNeighbours(), neighbours);
}