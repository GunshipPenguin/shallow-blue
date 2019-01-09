#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "uci.h"
#include "attacks.h"
#include "movepicker.h"
#include "eval.h"
#include "rays.h"

int main(int argc, char *argv[]) {
  Rays::init();
  PSquareTable::init();
  ZKey::init();
  MovePicker::init();
  Attacks::init();
  Eval::init();
  Uci::init();

  int result = Catch::Session().run(argc, argv);

  return result;
}
