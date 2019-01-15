#ifndef PAWNSTRUCTURETABLE_H
#define PAWNSTRUCTURETABLE_H

#include "board.h"

/**
 * @namespace PawnStructureTable
 * @brief Namespace holding things related to the pawn structure table
 *
 * This is a hash table to store evaluation scores of pawn structures. As
 * pawn structure doesn't often change in a search, evaluation can be sped
 * up by storing old scores in a table.
 */
namespace PawnStructureTable {
/**
 * @brief An entry in the pawn structure table
 */
struct PawnStructureEntry {

  /**
   * @brief Array indexed by [GamePhase] of pawn structure scores for this
   * entry
   */
  int scores[2];
};

/**
 * @brief Gets a pointer to the pawn structure table entry with the given ZKey, or
 * nullptr if it does not exist
 *
 * @param key ZKey to search pawn structure table for
 * @return A Pointer to the PawnStructureEntry with the given ZKey, or nullptr
 * if it does not exist
 */
PawnStructureEntry *get(ZKey key);

/**
 * @brief Returns true if the pawn structure table contains an entry with the
 * given ZKey, false otherwise
 *
 * @return true if the pawn structure table contains an entry with the given
 * ZKey, false otherwise
 */
bool exists(ZKey);

/**
 * @brief Creates a new entry in the pawn structure table
 *
 * @param key ZKey of new entry
 * @param entry New entry to store in table
 */
void set(ZKey, PawnStructureEntry);
};

#endif
