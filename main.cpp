#include <iostream>
#include <Project.h>


int main() {
  Project project(".");


/*
  std::cout << "digraph {\n";
  for (auto& Module : project.getModules()) {
    std::cout << "\n  // module " << Module.getName() << "\n";
    for (auto& Dependency : Module.getDependencyInformation()) {
      int weight = Dependency.second.weight;
      std::cout << "  " << Module.getName() << " -> " << Dependency.first->getName() << "[label=\"" << weight <<
                        "\",weight=\"" << weight << "\"]\n";
    }
  }
  std::cout << "}\n";
  */
}