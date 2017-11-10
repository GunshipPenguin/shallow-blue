#include "uci.h"
#include "psquaretable.h"
#include "zkey.h"
#include "raytable.h"
#include "movepicker.h"

int main() {
  RayTable::init();
  PSquareTable::init();
  ZKey::init();
  MovePicker::init();

  Uci uci;
  uci.start();

  return 0;
}
