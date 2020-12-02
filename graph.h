#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

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
    // Gets all vertices where an edge exists from the source to the vertex
    std::vector<std::string> GetDestinations(const std::string& source);
    bool VertexExists(const std::string& key);
    bool EdgeExists(const std::string& source, const std::string& dest);
    std::pair<std::vector<std::string>, std::vector<std::string>> BFS();
    void BFS(std::string start, std::vector<std::string>& discovery, std::vector<std::string>& cross, std::unordered_set<std::string>& visited);
  private:
    std::unordered_map<std::string, Vertex> vertices;
    std::unordered_map<std::string, Edge> edges;
    std::unordered_map<std::string, std::vector<std::string>> adj_list;
};