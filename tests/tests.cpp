#include "../catch/catch.hpp"
#include <fstream>
#include <string>
#include <iostream>

#include "../graph.h"

TEST_CASE("Checking invalid argument detection in vertex constructor", "[vertex][graph]") {
  Graph g("invalid-airports.dat", "sample_routes.dat");
  // Check that no vertices or edges are added to the graph when provided with invalid inputs
  REQUIRE(g.GetNumVertices() == 0);
  REQUIRE(g.GetNumEdges() == 0);
}