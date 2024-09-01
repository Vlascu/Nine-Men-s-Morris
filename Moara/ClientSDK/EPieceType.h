/**
 * @file EPieceType.h
 * @brief Defines the types of pieces used in the game.
 *
 * This file contains the definition of the `EPieceType` enumeration, which specifies the possible types of pieces in the game.
 */

#pragma once
#include <stdint.h>

 /**
  * @enum EPieceType
  * @brief Enumeration for specifying the type of pieces in the game.
  *
  * This enumeration defines the possible types of pieces that can be used in the game.
  */
enum class EPieceType : uint8_t
{
    Black, /**< Black pieces. */
    White, /**< White pieces. */
    Red,   /**< Red pieces. */
    Yellow, /**< Yellow pieces. */
    Count, /**< Total count of piece types (for internal use). */
    None   /**< No piece (used for empty nodes). */
};
