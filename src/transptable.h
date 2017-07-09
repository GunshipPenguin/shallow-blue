#ifndef TRANSPTABLE_H
#define TRANSPTABLE_H

#include "board.h"
#include "zkey.h"
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
  void set(ZKey, int, int, Flag);

  /**
   * @brief Returns true if the transposition table contains the given ZKey.
   * @param  key ZKey to lookup
   * @return true if there is an entry in the transposition table with the ZKey, false otherwise
   */
  bool contains(const ZKey&) const;

  /**
   * @brief Returns the score of the entry with the given ZKey in the transposition table.
   *
   * Throws an exception if the entry does not exist.
   *
   * @param  key ZKey of the entry to get the score of
   * @return The score of the entry with the given ZKey
   */
  int getScore(const ZKey&) const;

  /**
   * @brief Returns the depth of the entry with the given ZKey in the transposition table.
   *
   * Throws an exception if the entry does not exist.
   *
   * @param  key ZKey of the entry to get the depth of
   * @return The depth of the entry with the given ZKey
   */
  int getDepth(const ZKey&) const;

  /**
   * @brief Returns the flag of the entry with the given ZKey in the transposition table.
   *
   * Throws an exception if the entry does not exist.
   *
   * @param  key ZKey of the entry to get the flag of
   * @return The flag of the entry with the given ZKey
   */
  Flag getFlag(const ZKey&) const;

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
  std::unordered_map<U64, std::tuple<int, int, Flag> > _table;
};

#endif
