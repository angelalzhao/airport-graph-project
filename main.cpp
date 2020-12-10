#include <iostream>

#include "graph.h"

int main() {
  // Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  Graph g("data/airports.dat", "data/routes.dat");
  std::vector<std::string> c = g.BFS();
  std::cout << c.size() << " airports traversed with BFS" << std::endl << std::endl;

  std::cout << "Shortest path between SFO and SCL is:" << std::endl;
  const auto & path = g.Dijkstras("SFO", "SCL");
  std::cout << path.first << std::endl << std::endl;

  std::cout << "Airports sorted by PageRank score:" << std::endl;
  const auto & rank = g.PageRank();
  for (size_t i = 0; i < 10; i++) {
    std::cout << rank.second[i].second << " - " << rank.second[i].first << " " << std::endl;
  }
}