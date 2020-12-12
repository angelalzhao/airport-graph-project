#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>

#include "utils.h"

class Vertex {
  public:
    Vertex();
    // Constructs a vertex from one line from airport dataset
    Vertex(const std::string& csv);
    std::string GetKey() const;
    std::string GetName() const;
    std::string GetCity() const;
    Coord GetCoords() const;
  private:
    // 3 letter airport code
    std::string key;
    std::string name;
    std::string city;
    Coord coords;
};
class Edge {
  public:
    Edge();
    // Constructs an edge from one line from routes dataset
    Edge(const std::string& csv);
    std::string GetKey() const;
    std::string GetSource() const;
    std::string GetDest() const;
    double GetWeight() const;
    void SetWeight(double new_weight);
  private:
    // [Source vertex key]-[destination vertex key]
    std::string key;
    std::string source;
    std::string dest;
    double weight;
};
class Graph {
  public:
    Graph(const std::string& vertex_file, const std::string& edge_file);
    unsigned GetNumVertices() const;
    unsigned GetNumEdges() const;
    // Gets vertex from airport code
    Vertex GetVertex(const std::string& code);
    // Gets all vertices where an edge exists from the source to the vertex
    std::vector<std::string> GetDestinations(const std::string& source);
    bool VertexExists(const std::string& key);
    bool EdgeExists(const std::string& source, const std::string& dest);
    void SetEdgeWeight(const std::string& source, const std::string& dest, double weight);
    // Traverses the whole graph including all connected components by looping through all vertices and performing and individual
    // BFS traversal starting at each unvisited vertex
    // Returns a vector of vertex keys (airport codes) in the order visited by the traversal
    std::vector<std::string> BFS();
    // Individual BFS traversal starting at specified point. Will not visit every node in graph if graph is not strongly connected
    // Returns a vector of vertex keys (airport codes) in the order visited by the traversal
    std::vector<std::string> BFS(const std::string& start);
    // Performs Dijkstra's algorithm to find the shortest path from start to end or to determine that no path exists
    // Returns a string representation of the shortest path or an indication that no path was found
    // Returns the distance corresponding the the shortest path (or infinity if no path found)
    std::pair<std::string, double> Dijkstras(const std::string& start, const std::string& end);
    // Performs PageRank and returns a map of each airport code to PageRank score, as well as a sorted list of airport codes ranked from
    // highest to lowest score (most to least popular airports according to the algorithm)
    std::pair<std::unordered_map<std::string, double>, std::vector<std::pair<double, std::string>>> PageRank();
  private:
    std::unordered_map<std::string, Vertex> vertices;
    std::unordered_map<std::string, Edge> edges;
    std::unordered_map<std::string, std::vector<std::string>> adj_list;
    void BFS(const std::string& start, std::vector<std::string>& v, std::unordered_set<std::string>& visited);
};