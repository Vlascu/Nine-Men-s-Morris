/**
 * @file EBoardType.h
 * @brief Defines board types for the game.
 *
 * This file contains the definition of the `EBoardType` enumeration, which specifies the possible types of the game board.
 */

#pragma once
#include <stdint.h>

 /**
  * @enum EBoardType
  * @brief Enumeration for specifying the type of the game board.
  *
  * This enumeration defines the possible types for the game board.
  */
enum class EBoardType : uint8_t
{
    Normal,    /**< Standard board type without diagonals. */
    Diagonals,  /**< Board type with diagonals. */
    None
};
