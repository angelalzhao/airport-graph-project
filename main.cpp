#include <iostream>

#include "graph.h"

int main() {
  Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  //Graph g("data/airports.dat", "data/routes.dat");
  std::vector<std::string> c = g.BFS();
  for (std::string s : c) {
    std::cout << s << std::endl;
  }

  Coord first(37.773972, -122.431297);
  Coord second(40.730610, -73.935242);
  std::cout << Distance(first, second) << std::endl;

}