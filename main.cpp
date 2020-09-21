#include <iostream>
#include <memory>
#include "Map.h"

int main() {
  std::cout << "Hello World!" << std::endl;
  Map hello = new Map();

  std::shared_ptr<Territory> t1 = hello.add(Territory(0, "Mamie", "me"));
  hello.add(Continent(0, "Mamie2", "me"));

  std::shared_ptr<Territory> t2 = hello.add(Territory(1, "Mamie3", "me"));

  hello.link(t1, t2);

  hello.validate();

  std::cout << hello.to_string() << std::endl;

  for (int b : t1->borders) {
      std::cout << b << std::endl;
  }

  for (int b : t2->borders) {
      std::cout << b << std::endl;
  }

  std::shared_ptr<Territory> p = hello.findTerritory("Mamie");
  p->units = 1;
  
  p

  std::cout << hello.to_string() << std::endl;
}