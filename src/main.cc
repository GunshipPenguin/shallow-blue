#include "uci.h"
#include "psquaretable.h"
#include "zkey.h"
#include "raytable.h"

int main() {
  RayTable::init();
  PSquareTable::init();
  ZKey::init();

  Uci uci;
  uci.start();

  return 0;
}
