/**
 * @file IBoardComputer.h
 * @brief Interface for board computation logic used by the computer player.
 *
 * This file defines the `IBoardComputer` interface, which provides methods for calculating possible moves and game states for the computer player.
 */

#pragma once
#include "EPieceType.h"

#include <vector>

using NodeList = std::vector<class INode*>;
using PieceIndexes = std::vector<uint8_t>;
using MovingPlayerPtr = std::shared_ptr<class IPlayer>;
using EPlayerType = EPieceType;

/**
 * @class IBoardComputer
 * @brief Interface for calculating moves and game states for the computer player.
 *
 * This class defines methods for retrieving nodes, calculating possible moves and places for pieces,
 * and checking for line formations on the game board, specifically for use by the computer player.
 */
class IBoardComputer
{
public:
    /**
     * @brief Retrieves all nodes on the game board.
     * @return A list of pointers to all nodes.
     */
    virtual NodeList GetAllNodes() const = 0;

    /**
     * @brief Gets the possible positions where pieces can be placed.
     * @return A list of indexes representing possible placement positions.
     */
    virtual PieceIndexes GetPossiblePlaces() const = 0;

    /**
     * @brief Gets possible moves from a specific node for a given player.
     * @param nodeIndex The index of the node to query.
     * @param player The type of the player making the move.
     * @return A list of indexes representing possible moves from the specified node.
     */
    virtual PieceIndexes GetPossibleMovesFromNode(uint8_t nodeIndex, EPlayerType player) const = 0;

    /**
     * @brief Gets all possible moves for a given player.
     * @param player The type of the player making the moves.
     * @return A list of indexes representing all possible moves for the specified player.
     */
    virtual PieceIndexes GetPossibleMoves(EPlayerType player) const = 0;

    /**
     * @brief Gets all possible pieces that can be removed for a given player.
     * @param player The type of the player whose pieces can be removed.
     * @return A list of indexes representing all possible removable pieces for the specified player.
     */
    virtual PieceIndexes GetPossibleRemoves(EPlayerType player) const = 0;

    /**
     * @brief Checks if a node can be part of a full line.
     * @param nodeIndex The index of the node to check.
     * @param nodePieceType The type of the piece at the node (default is EPieceType::None).
     * @param indexToIgnore The index to ignore in the check (default is 255).
     * @return True if the node can be part of a full line, false otherwise.
     */
    virtual bool CanBeFullLine(uint8_t nodeIndex, EPieceType nodePieceType = EPieceType::None, uint8_t indexToIgnore = 255) const = 0;

    /**
     * @brief Checks if a node is part of a full line.
     * @param nodeIndex The index of the node to check.
     * @param currentNodeType The type of the piece at the node (default is EPieceType::None).
     * @param indexToIgnore The index to ignore in the check (default is 255).
     * @param windmill True if checking for a windmill formation, false otherwise.
     * @return True if the node is part of a full line, false otherwise.
     */
    virtual bool IsFullLine(uint8_t nodeIndex, EPieceType currentNodeType = EPieceType::None, uint8_t indexToIgnore = 255, bool windmill = false) const = 0;
};
