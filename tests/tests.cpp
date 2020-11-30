#include "../catch/catch.hpp"
#include <fstream>
#include <string>
#include <iostream>

#include "../graph.h"
// Tests not working??? No vertices and edges are getting added??? Code seems to work fine in main
// Issue is probably with file reading???
TEST_CASE("Checking invalid argument detection in vertex constructor", "[vertex][graph]") {
  Graph g("invalid-airports.dat", "sample_routes.dat");
  // Check that no vertices or edges are added to the graph when provided with invalid inputs
  REQUIRE(g.GetNumVertices() == 0);
  REQUIRE(g.GetNumEdges() == 0);
}

TEST_CASE("Parsing a small graph", "[graph]") {
  Graph g("sample_airports.dat", "sample_routes.dat");
  REQUIRE(g.GetNumVertices() == 10);
  REQUIRE(g.GetNumEdges() == 20);
  REQUIRE(g.VertexExists("ORD"));
}