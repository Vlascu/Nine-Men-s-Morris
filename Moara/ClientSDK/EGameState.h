/**
 * @file EGameState.h
 * @brief Defines the various states of the game.
 *
 * This file contains the definition of the `EGameState` enumeration, which specifies the possible states the game can be in.
 */

#pragma once
#include <stdint.h>

 /**
  * @enum EGameState
  * @brief Enumeration for specifying the state of the game.
  *
  * This enumeration defines the possible states for the game.
  */
enum class EGameState : uint8_t
{
    Placing,  /**< State where players are placing their pieces on the board. */
    Moving,   /**< State where players are moving their pieces. */
    Removing, /**< State where players are removing opponent's pieces. */
    Finished  /**< State where the game has finished. */
};
