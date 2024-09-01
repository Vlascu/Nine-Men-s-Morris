/**
 * @file IGameListener.h
 * @brief Interface for game event listeners.
 *
 * This file defines the `IGameListener` interface, which provides methods for responding to various game events.
 */

#pragma once

#include "EPieceType.h"
#include "EGameState.h"

#include <stdint.h>
#include <memory>

using GameListenerPtr = std::shared_ptr<class IGameListener>;
using ListenerWeakPtr = std::weak_ptr<class IGameListener>;

/**
 * @class IGameListener
 * @brief Interface for responding to game events.
 *
 * This class defines the methods that a game listener must implement to respond to various game events such as moves, piece additions, removals, state changes, and more.
 */
class IGameListener
{
public:
    /**
     * @brief Called when a move is made in the game.
     * @param fromNodeIndex The index of the node from which the piece was moved.
     * @param toNodeIndex The index of the node to which the piece was moved.
     * @param fromNodeType The type of piece that was moved.
     */
    virtual void OnMoveMade(uint8_t fromNodeIndex, uint8_t toNodeIndex, EPieceType fromNodeType) = 0;

    /**
     * @brief Called when a piece is added to the game.
     * @param addedNodeIndex The index of the node where the piece was added.
     * @param nodeType The type of piece that was added.
     */
    virtual void OnAddedPiece(uint8_t addedNodeIndex, EPieceType nodeType) = 0;

    /**
     * @brief Called when a piece is removed from the game.
     * @param removedNodeIndex The index of the node from which the piece was removed.
     */
    virtual void OnRemovedPiece(uint8_t removedNodeIndex) = 0;

    /**
     * @brief Called when the game state changes.
     * @param newState The new state of the game.
     */
    virtual void OnGameStateChanged(EGameState newState) = 0;

    /**
     * @brief Called when the windmill rule is triggered in the game.
     */
    virtual void OnWindmillRule() = 0;

    /**
     * @brief Called when the active player changes.
     * @param playerType The type of the new active player.
     * @param isComputer True if the new active player is the computer, false otherwise.
     */
    virtual void OnPlayerChanged(EPieceType playerType, bool isComputer) = 0;

    /**
     * @brief Called when a player is removed from the game.
     * @param who The type of the player who was removed.
     */
    virtual void OnPlayerRemoved(EPieceType who) = 0;
};
