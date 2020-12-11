#include <iostream>
#include <iomanip>

#include "graph.h"

int main() {
  Graph g("data/airports.dat", "data/routes.dat");
  std::string input;
  std::cout << "Welcome to our project demo!" << std::endl;
  std::cout << "Please enter a command. For a list of commands type `help`. Type `quit` to exit." << std::endl;
  while (true) {
    std::cout << "> ";
    std::cin >> input;
    if (input == "help") {
      std::cout << "Commands: bfs, dijkstra, pagerank" << std::endl;
    } else if (input == "bfs") {
      std::vector<std::string> traversal = g.BFS();
      std::cout << traversal.size() << " airports traversed with a breadth-first search. First ten:" << std::endl;
      for (size_t i = 0; i < 10; i++) {
        std::cout << traversal[i] << " – " << g.GetVertex(traversal[i]).GetName() << std::endl;
      }
    } else if (input == "dijkstra") {
      std::cout << "Provide 3 letter airport codes to find the shortest path between the two. For example, SFO (San Francisco) to CMI (Willard Airport)." << std::endl;
      std::string src, dest;
      std::cout << "Flying from: ";
      std::cin >> src;
      if (!g.VertexExists(src)){
        std::cout << "Airport not recognized" << std::endl;
        continue;
      }
      std::cout << "Flying to: ";
      std::cin >> dest;
      if (!g.VertexExists(dest)){
        std::cout << "Airport not recognized" << std::endl;
        continue;
      }
      const auto & path = g.Dijkstras(src, dest);
      std::cout << path.first << std::endl;
    } else if (input == "pagerank") {
      std::cout << "Running PageRank algorithm..." << std::endl;
      const auto & rank = g.PageRank();
      std::cout << "Top airports based on PageRank score:" << std::endl;
      for (size_t i = 0; i < 10; i++) {
        std::cout << rank.second[i].second << " (" << std::fixed << std::setprecision(5) << rank.second[i].first << ") – " << g.GetVertex(rank.second[i].second).GetName() << std::endl;
      }
    } else if (input == "quit") {
      return 0;
    } else {
      std::cout << "Command not recognized. Try again." << std::endl;
    }
  }
}