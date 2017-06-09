#include <iostream>
#include "raytable.h"

int main() {
  RayTable raytable;

  std::cout << std::hex << "0x" << raytable.getRay(RayTable::SOUTH_EAST, 27) << std::endl;
  std::cout << std::hex << "0x" << raytable.getRay(RayTable::SOUTH_EAST, 36) << std::endl;

  return 0;
}
