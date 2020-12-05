#include "../catch/catch.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

#include "../graph.h"
#include "../utils.h"
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

TEST_CASE("Testing BFS traversal", "[bfs][graph]") {
  Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  std::vector<std::string> c = g.BFS();
  REQUIRE(c.size() == 10);

  std::vector<std::string> test;
  test.push_back("SEA");
  test.push_back("LAS");
  test.push_back("CLT");
  test.push_back("SFO");
  test.push_back("ATL");
  test.push_back("JFK");
  test.push_back("LAX");
  test.push_back("DEN");
  test.push_back("ORD");
  test.push_back("DFW");

  for (int i = 0; i < c.size(); i++) {
    REQUIRE(c.at(i) == test.at(i));
  }
}


TEST_CASE("Test distance function", "[dist][utils]") {
  Coord first(37.773972, -122.431297);
  Coord second(40.730610, -73.935242);
  
  REQUIRE(Distance(first, second) == 4135.54);
}
