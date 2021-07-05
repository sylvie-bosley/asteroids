#include "asteroids.h"
#include "helpers.h"

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc > 1)
    std::cout << "Hello " << Asteroids::capitalize(argv[1]) << "!\n";
  else
    std::cout << "Hello user!\n";

  return 0;
}
