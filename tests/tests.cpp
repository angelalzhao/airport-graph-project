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