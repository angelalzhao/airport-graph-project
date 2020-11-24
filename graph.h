#pragma once
#include <string>
#include <vector>
#include <unordered_map>
class Vertex {
  public:
    Vertex(const std::string& csv);
    std::string GetKey() const;
    std::string GetName() const;
    std::string GetCity() const;
    std::pair<double, double> GetCoords() const;
  private:
    std::string key;
    std::string name;
    std::string city;
    std::pair<double, double> coords;
};
class Edge {
  public:
    Edge(const std::string& csv);
    std::string GetKey() const;
    std::string GetSource() const;
    std::string GetDest() const;
    double GetWeight() const;
  private:
    std::string key;
    std::string source;
    std::string dest;
    double weight;
};
class Graph {
  public:
    Graph(const std::string& filename);
  private:
    std::unordered_map<std::string, Vertex> vertices;
    std::unordered_map<std::string, Edge> edges;
    std::unordered_map<std::string, std::vector<std::string>> adj_list;
};