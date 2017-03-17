#include <iostream>
#include <Module.h>


int main() {
  std::cout << "Hello, World!" << std::endl;
  Module m;
  m.parseDirectory(".");
  return 0;
}