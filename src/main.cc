#include "uci.h"
#include "psquaretable.h"
#include "zkey.h"

int main() {
  PSquareTable::init();
  ZKey::init();

  Uci uci;
  uci.start();

  return 0;
}
