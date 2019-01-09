#include "rays.h"
#include "bitutils.h"

U64 Rays::detail::_rays[8][64];

void Rays::init() {
  for (int square = 0; square < 64; square++) {
    // North
    detail::_rays[NORTH][square] = 0x0101010101010100ULL << square;

    // South
    detail::_rays[SOUTH][square] = 0x0080808080808080ULL >> (63 - square);

    // East
    detail::_rays[EAST][square] = 2 * ((ONE << (square | 7)) - (ONE << square));

    // West
    detail::_rays[WEST][square] = (ONE << square) - (ONE << (square & 56));

    // North West
    detail::_rays[NORTH_WEST][square] = _westN(0x102040810204000ULL, 7 - _col(square)) << (_row(square) * 8);

    // North East
    detail::_rays[NORTH_EAST][square] = _eastN(0x8040201008040200ULL, _col(square)) << (_row(square) * 8);

    // South West
    detail::_rays[SOUTH_WEST][square] = _westN(0x40201008040201ULL, 7 - _col(square)) >> ((7 - _row(square)) * 8);

    // South East
    detail::_rays[SOUTH_EAST][square] = _eastN(0x2040810204080ULL, _col(square)) >> ((7 - _row(square)) * 8);
  }
}

U64 Rays::getRay(Dir dir, int square) {
  return detail::_rays[dir][square];
}
