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
