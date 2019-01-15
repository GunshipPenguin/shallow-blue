#ifndef TRANSPTABLE_H
#define TRANSPTABLE_H

#include "board.h"
#include "zkey.h"
#include "transptableentry.h"
#include <unordered_map>

/**
 * @brief A transposition table.
 *
 * Each entry is mapped to by a ZKey and contains a score, depth and flag which
 * indicates if the stored score is an upper bound, lower bound or exact score.
 *
 */
class TranspTable {
 public:
  /**
   * @enum Flag
   * @brief Flag stored with each transposition table entry indicating its type.
   */
  enum Flag {
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
  };

  /**
   * @brief Constructs a new empty transposition table.
   */
  TranspTable() = default;;

  /**
   * @brief Creates a new entry in the transposition table.
   *
   * If the entry already exists, it will be overwritten.
   *
   * @param key Zobrist key of the board
   * @param entry Entry to store
   */
  void set(const ZKey &, TranspTableEntry);

  /**
   * @brief Get the entry in the transposition table for the given ZKey
   * returns nullptr if no entry exists for the given ZKey.
   *
   * @param key ZKey to lookup entry for
   * @return The transposition table entry corresponding to the given ZKey, or nullptr if it does not exist
   */
  const TranspTableEntry *getEntry(const ZKey &) const;

  /**
   * @brief Removes all entries from the transposition table.
   */
  void clear();

 private:
  /**
   * @brief The transposition table hash map.
   *
   * A map of ZKeys to a tuple of (score, depth, flag).
   */
  std::unordered_map<U64, TranspTableEntry> _table;
};

#endif
