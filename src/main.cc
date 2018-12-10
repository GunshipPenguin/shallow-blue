#include "uci.h"
#include "attacks.h"
#include "movepicker.h"
#include "eval.h"

int main() {
  PSquareTable::init();
  ZKey::init();
  MovePicker::init();
  Attacks::init();
  Eval::init();
  Uci::init();

  Uci::start();

  return 0;
}
