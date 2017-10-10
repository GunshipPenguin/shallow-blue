#ifndef TRANSPTABLE_H
#define TRANSPTABLE_H

#include "board.h"
#include "zkey.h"
#include "transptable_entry.h"
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
   * @brief Creates a new entry in the transposition table.
   *
   * If the entry already exists, it will be overwritten.
   *
   * @param key Zobrist key of the board
   * @param score  Score of position
   * @param depth  Depth of entry
   * @param flag Type of entry
   */
  void set(const ZKey&, TranspTableEntry);

  const TranspTableEntry* getEntry(const ZKey&) const;

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
