/**
 * @file IGame.h
 * @brief Interface for game logic and management.
 *
 * This file defines the `IGame` interface, which provides methods for managing the game state,
 * manipulating game pieces, and handling game events.
 */

#pragma once


#include "EBoardType.h"
#include "EBoardSize.h"
#include "EComputerLevel.h"
#include "EGameState.h"
#include "EPieceType.h"

#include "IGameConfig.h"
#include "IGameListener.h"
#include "IComputerLevel.h"

#include "FileException.h"

#include <vector>
#include <string>
#include <memory>

struct PlayerConfig;

using NodeList = std::vector<class INode*>;
using PieceTypeList = std::vector<EPieceType>;
using IGamePtr = std::shared_ptr<class IGame>;
using EPlayerType = EPieceType;
using PlayerTypeList = std::vector<EPlayerType>;
using PieceIndexes = std::vector<uint8_t>;
using ListenerSharedPtr = std::shared_ptr<IGameListener>;

/**
 * @class IGame
 * @brief Interface for a game, providing methods for game management and operations.
 *
 * This class defines the operations and properties required for managing a game, including
 * initialization, querying game state, manipulating game pieces, and interacting with game listeners.
 */
class IGame
{
public:
    /**
     * @brief Creates a new game instance based on the provided configuration.
     * @param gameConfig Configuration settings for the game.
     * @return A shared pointer to the newly created game instance.
     */
    static IGamePtr Produce(GameConfigPtr& gameConfig);

    /**
     * @brief Initializes the game with the specified configuration.
     * @param gameConfig Configuration settings for the game.
     */
    virtual void Initialize(GameConfigPtr& gameConfig) = 0;

    /**
     * @brief Retrieves the current state of the game.
     * @return The current game state.
     */
    virtual EGameState GetGameState() const = 0;

    /**
     * @brief Gets the type of the winning player.
     * @return The piece type representing the winning player.
     */
    virtual EPieceType GetWinningPlayer() const = 0;

    /**
     * @brief Gets the type of the active player.
     * @return The piece type representing the active player.
     */
    virtual EPieceType GetActivePlayer() const = 0;

    /**
     * @brief Gets index of the active player.
     * @return The index representing of the active player.
     */
    virtual int GetActivePlayerIndex() const = 0;

    /**
     * @brief Gets the piece type at a specified node index.
     * @param nodeIndex Index of the node to query.
     * @return The piece type at the specified node.
     */
    virtual EPieceType GetPiece(uint8_t nodeIndex) const = 0;

    /**
     * @brief Retrieves a list of all pieces currently in the game.
     * @return A list of all piece types present in the game.
     */
    virtual PieceTypeList GetAllPieces() const = 0;

    /**
     * @brief Gets the node type at a specified index.
     * @param nodeIndex Index of the node to query.
     * @return The piece type at the specified node.
     */
    virtual EPieceType GetNode(uint8_t nodeIndex) const = 0;

    /**
     * @brief Retrieves a list of all nodes in the game.
     * @return A list of pointers to all nodes.
     */
    virtual NodeList GetAllNodes() const = 0;

    /**
     * @brief Retrieves a list of all player types in the game.
     * @return A list of all player piece types.
     */
    virtual PlayerTypeList GetAllPlayerTypes() const = 0;

    /**
     * @brief Gets the number of pieces a player currently has on the board.
     * @param player The piece type representing the player.
     * @return The number of pieces of the specified player on the board.
     */
    virtual uint8_t GetPlayerPiecesOnBoard(EPieceType player) const = 0;

    /**
     * @brief Gets the number of pieces a player still needs to place on the board.
     * @param player The piece type representing the player.
     * @return The number of pieces of the specified player yet to be placed.
     */
    virtual uint8_t GetPlayerPiecesToPlace(EPieceType player) const = 0;

    /**
     * @brief Retrieves possible moves from a specified node index.
     * @param nodeIndex Index of the node to query.
     * @return A list of possible moves from the specified node.
     */
    virtual PieceIndexes GetPossibleMovesFromNode(uint8_t nodeIndex) const = 0;

    /**
     * @brief Retrieves a list of possible pieces that can be removed.
     * @return A list of indexes of pieces that can be removed.
     */
    virtual PieceIndexes GetPossibleRemoves() const = 0;

    /**
     * @brief Retrieves the time elapsed for the current round.
     * @return The elapsed time in milliseconds for the current round.
     */
    virtual double GetRoundTime() const = 0;

    /**
     * @brief Checks and updates the state of the game to determine if there is a winning player.
     */
    virtual void CheckWiningPlayer() = 0;

    /**
     * @brief Adds a listener to the game.
     * @param listener A shared pointer to the listener to be added.
     */
    virtual void AddListener(ListenerSharedPtr listener) = 0;

    /**
     * @brief Removes a specific listener from the game.
     * @param listener A weak pointer to the listener to be removed.
     */
    virtual void RemoveListener(ListenerWeakPtr listener) = 0;

    /**
     * @brief Removes all listeners from the game.
     */
    virtual void RemoveAllListeners() = 0;

    /**
     * @brief Places a piece at a specified node index.
     * @param nodeIndex Index of the node where the piece will be placed.
     */
    virtual void PlacePiece(uint8_t nodeIndex) = 0;

    /**
     * @brief Moves a piece from one node to another.
     * @param fromIndex Index of the node where the piece is moved from.
     * @param toIndex Index of the node where the piece is moved to.
     */
    virtual void MovePiece(uint8_t fromIndex, uint8_t toIndex) = 0;

    /**
     * @brief Removes a piece from a specified node index.
     * @param nodeIndex Index of the node where the piece will be removed.
     */
    virtual void RemovePiece(uint8_t nodeIndex) = 0;

    /**
     * @brief Undoes the last move or action performed in the game.
     */
    virtual void Undo() = 0;

    /**
     * @brief Loads a game from a specified file.
     * @param filePath Path to the file from which the game will be loaded.
     */
    virtual void LoadGame(const std::string& filePath) = 0;

    /**
     * @brief Saves the current game state to a specified file.
     * @param filePath Path to the file where the game state will be saved.
     */
    virtual void SaveGame(const std::string& filePath) const = 0;

    /**
     * @brief Allows the computer to make a move.
     */
    virtual void LetComputerPlay() = 0;

    /**
     * @brief Checks if it is currently the computer's turn.
     * @return True if it is the computer's turn, false otherwise.
     */
    virtual bool IsComputerTurn() = 0;

    /**
     * @brief Sets the level of the computer player using a provided pointer.
     * @param computerLevel A shared pointer to the computer level to be set.
     */
    virtual void SetComputerLevel(ComputerLevelPtr computerLevel) = 0;

    /**
     * @brief Sets the level of the computer player using an enumerator.
     * @param level An enumerator representing the desired computer level.
     */
    virtual void SetComputerLevel(EComputerLevel level) = 0;

    /**
     * @brief Sets the player config for the game.
     * @param config A configuration that specifies the type of player and if they are human or computer.
     */
    virtual void SetPlayerConfig(PlayerConfig& config) = 0;

    virtual ~IGame() = default;
};
