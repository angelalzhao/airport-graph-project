#include "../catch/catch.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

#include "../graph.h"
// Tests not working??? No vertices and edges are getting added??? Code seems to work fine in main
// Issue is probably with file reading???
TEST_CASE("Checking invalid argument detection in vertex constructor", "[vertex][graph]") {
  Graph g("tests/invalid-airports.dat", "tests/sample_routes.dat");
  // Check that no vertices or edges are added to the graph when provided with invalid inputs
  REQUIRE(g.GetNumVertices() == 0);
  REQUIRE(g.GetNumEdges() == 0);
}

TEST_CASE("Parsing a small graph", "[graph]") {
  Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  REQUIRE(g.GetNumVertices() == 10);
  REQUIRE(g.GetNumEdges() == 20);
  REQUIRE(g.VertexExists("ORD"));
  REQUIRE(g.EdgeExists("ORD", "LAX"));
  REQUIRE(g.EdgeExists("LAX", "ORD"));
  REQUIRE(!g.VertexExists("LGA"));
  REQUIRE(!g.EdgeExists("ORD", "CLT"));
  std::vector<std::string> neighbors = g.GetDestinations("ORD");
  std::sort(neighbors.begin(), neighbors.end());
  REQUIRE(neighbors.size() == 2);
  REQUIRE(neighbors[0] == "DFW");
  REQUIRE(neighbors[1] == "LAX");
}

TEST_CASE("Testing BFS traversal", "[bfs]") {
  Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  std::vector<std::string> c = g.BFS().second;
  REQUIRE(c.size() == 11);

  std::vector<std::string> test;
  test.push_back("LAS-SEA");
  test.push_back("CLT-SEA");
  test.push_back("SFO-LAS");
  test.push_back("ATL-CLT");
  test.push_back("JFK-SFO");
  test.push_back("LAX-ATL");
  test.push_back("DEN-JFK");
  test.push_back("ORD-LAX");
  test.push_back("ORD-DFW");
  test.push_back("DFW-ORD");
  test.push_back("DFW-DEN");

  for (int i = 0; i < c.size(); i++) {
    REQUIRE(c.at(i) == test.at(i));
  }
}
