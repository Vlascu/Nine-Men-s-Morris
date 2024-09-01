/**
 * @file EBoardSize.h
 * @brief Enumeration for board sizes.
 *
 * This file defines the `EBoardSize` enumeration, which specifies different sizes for the game board.
 */

#pragma once
#include <stdint.h>

 /**
  * @enum EBoardSize
  * @brief Enumeration representing the possible board sizes.
  *
  * This enumeration defines the different sizes that a game board can have.
  */
enum class EBoardSize : uint8_t
{
    /** A normal-sized board. */
    Normal,

    /** A large-sized board. */
    Big,
    None
};
