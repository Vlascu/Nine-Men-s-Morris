#pragma once
#include "EComputerLevel.h"

#include "PlayerConfig.h"
#include "BoardConfig.h"

#include <memory>

using GameConfigPtr = std::shared_ptr<class IGameConfig>;

/**
 * @brief Interface for game configuration management.
 */
class IGameConfig
{
public:
    /**
     * @brief Set the configuration for a player.
     *
     * @param config The player configuration.
     */
    virtual void SetPlayerConfig(PlayerConfig& config) = 0;

    /**
     * @brief Set the board configuration.
     *
     * @param type The type of the board.
     * @param size The size of the board.
     */
    virtual void SetBoardConfig(EBoardType type, EBoardSize size) = 0;

    /**
     * @brief Set the computer AI level.
     *
     * @param level The computer level.
     */
    virtual void SetComputerLevel(EComputerLevel level) = 0;

    /**
     * @brief Set the number of players in the game.
     *
     * @param numberOfPlayers The number of players.
     */
    virtual void SetNumberOfPlayers(int numberOfPlayers) = 0;

    /**
     * @brief Set the configurations for multiple players.
     *
     * @param matrix The matrix containing player configurations.
     * @param indexOfMatrix The index in the matrix to use.
     */
    virtual void SetPlayersConfig(ConfigurationsMatrix matrix, int indexOfMatrix) = 0;

    /**
     * @brief Get the board configuration matrix.
     *
     * @return The matrix containing board configurations.
     */
    virtual BoardConfigMatrix GetBoardConfigMatrix() const = 0;

    /**
     * @brief Get the type of the board.
     *
     * @return The type of the board.
     */
    virtual EBoardType GetBoardType() const = 0;

    /**
     * @brief Get the size of the board.
     *
     * @return The size of the board.
     */
    virtual EBoardSize GetBoardSize() const = 0;

    /**
     * @brief Get the configuration of a player.
     *
     * @return The player configuration.
     */
    virtual Config GetPlayerConfig() const = 0;

    /**
     * @brief Get the number of pieces a player needs to place.
     *
     * @return The number of pieces to place.
     */
    virtual int GetNumberOfPiecesToPlace() const = 0;

    /**
     * @brief Get the number of players in the game.
     *
     * @return The number of players.
     */
    virtual int GetNumberOfPlayers() const = 0;

    /**
     * @brief Get the computer AI level.
     *
     * @return The computer level.
     */
    virtual EComputerLevel GetComputerLevel() const = 0;

    /**
  * @brief Get the name of the board matrix config.
  *
  * @param size The size of the board.
  * @param type The type of the board.
  * @return The name of the board config matrix on wich the game will be build.
  */
    virtual std::string GetBoardMatrixName(EBoardSize size, EBoardType type) = 0;

    virtual void SetPiecesToPlace(int number) = 0;

    virtual void SetPlayersConfig(Config& config) = 0;
};
