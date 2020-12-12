#include "../catch/catch.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

#include "../graph.h"
#include "../utils.h"

/* Test data:
invalid-airports.dat - contains several airport data entries that would not be considered valid by our code
sample_airports.dat and sample_routes.dat represent a small graph connecting 10 airports in a 10-cycle with edges
going both directions (10 vertices, 20 edges)
airports_small.dat and routes_small.dat represent a graph with two connected components:
One connected component consists of the single node JFK. The other component consists of ORD, ATL, CLT, LAX, 
with edges from ORD->CLT, CLT->ATL, ORD->LAX, LAX->ATL
*/

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
  std::vector<std::string> c = g.BFS("SEA");
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

TEST_CASE("BFS on small, unconnected graph", "[bfs][graph]") {
  Graph g("tests/airports_small.dat", "tests/routes_small.dat");
  auto full_bfs = g.BFS();
  // Verify that all vertices are visited
  REQUIRE(full_bfs.size() == 5);
  std::sort(full_bfs.begin(), full_bfs.end());
  std::vector<std::string> all_vertices{"ATL", "CLT", "JFK", "LAX", "ORD"};
  REQUIRE(full_bfs == all_vertices);
  // Checking JFK component
  auto jfk_bfs = g.BFS("JFK");
  REQUIRE(jfk_bfs.size() == 1);
  REQUIRE(jfk_bfs.at(0) == "JFK");
  // Checking ORD component
  auto ord_bfs = g.BFS("ORD");
  std::vector<std::string> ord_inorder{"ORD", "LAX", "CLT", "ATL"};
  REQUIRE(ord_bfs == ord_inorder);
}


TEST_CASE("Test distance function", "[dist][utils]") {
  Coord first(37.773972, -122.431297);
  Coord second(40.730610, -73.935242);
  
  REQUIRE((int) Distance(first, second) == 4135);
}

TEST_CASE("Test dijkstras function", "[dijkstras][graph]") {
  Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  std::string dij = g.Dijkstras("ORD", "JFK").first;
  REQUIRE(dij == "ORD -> DFW -> DEN -> JFK");
}

TEST_CASE("Dijkstras on small unconnected graph", "[dijkstras][graph]") {
  Graph g("tests/airports_small.dat", "tests/routes_small.dat");
  // Shortest path from ORD to ATL is through CLT
  auto ord_atl = g.Dijkstras("ORD", "ATL").first;
  REQUIRE(ord_atl == "ORD -> CLT -> ATL");
  // No path between ORD and JFK
  auto ord_jfk = g.Dijkstras("ORD", "JFK");
  REQUIRE(ord_jfk.first == "No path found");
  REQUIRE(ord_jfk.second == std::numeric_limits<double>::max());
}

TEST_CASE("Dijkstras chooses multiple node path over direct path", "[dijkstras][graph]") {
  Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  // Setting edge weights so that direct path from ORD to LAX is actually longer than 9 edge path
  g.SetEdgeWeight("ORD", "LAX", 10);
  g.SetEdgeWeight("ORD", "DFW", 1);
  g.SetEdgeWeight("DFW", "DEN", 1);
  g.SetEdgeWeight("DEN", "JFK", 1);
  g.SetEdgeWeight("JFK", "SFO", 1);
  g.SetEdgeWeight("SFO", "LAS", 1);
  g.SetEdgeWeight("LAS", "SEA", 1);
  g.SetEdgeWeight("SEA", "CLT", 1);
  g.SetEdgeWeight("CLT", "ATL", 1);
  g.SetEdgeWeight("ATL", "LAX", 1);
  std::string path = g.Dijkstras("ORD", "LAX").first;
  REQUIRE(path == "ORD -> DFW -> DEN -> JFK -> SFO -> LAS -> SEA -> CLT -> ATL -> LAX");
}

TEST_CASE("Page rank on sample data", "[pagerank][graph]") {
  Graph g("tests/sample_airports.dat", "tests/sample_routes.dat");
  const auto & rank = g.PageRank();
  //In a circular connected graph, each with one outgoing edge, the ranks are the same
  for (size_t i = 0; i < 10; i++) {
    REQUIRE(rank.second[i].first == 0.1);
  }
}

TEST_CASE("Page rank on small unconnected graph", "[pagerank][graph]") {
  Graph g("tests/airports_small.dat", "tests/routes_small.dat");
  const auto & rank = g.PageRank();
  //ensure that page rank still assigns a rank to vertices with no outgoing edges
  std::vector<double> test;
  test.push_back(0.41372);
  test.push_back(0.1722572757);
  test.push_back(0.1722572757);
  test.push_back(0.1208825114);
  test.push_back(0.1208825114);

  for (size_t i = 0; i < 5; i++) {
   REQUIRE(Approx(rank.second[i].first) == test[i]);
  }
}
