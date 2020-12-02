#include "graph.h"
#include <iostream>
int main() {
  Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  //Graph g("data/airports.dat", "data/routes.dat");
  std::vector<std::string> c = g.BFS().second;
  for (std::string s : c) {
    std::cout << s << std::endl;
  }
}