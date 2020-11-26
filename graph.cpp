#include "graph.h"
#include <sstream>
#include <fstream>
#include <iostream>
Vertex::Vertex() {}
// TODO: overload << operator for pretty printing ???
Vertex::Vertex(const std::string& csv) {
  // https://www.tutorialspoint.com/parsing-a-comma-delimited-std-string-in-cplusplus
  std::vector<std::string> parsed;
  std::stringstream ss(csv);
  while (ss.good()) {
    std::string substring;
    std::getline(ss, substring, ',');
    parsed.push_back(substring);
  }
  // TODO: remove quotes around key, name, city (if key is not null)
  key = parsed[4];

  name = parsed[1];
  city = parsed[2] + ", " + parsed[3];
  // TODO: add exception handling. currently throws exception somewhere when used on full dataset. should discard entries with invalid coordinates
  //coords.first = std::stod(parsed[6]);
  //coords.second = std::stod(parsed[7]);
  //std::cout << key << ", " << city << ", " << coords.first << ", " << coords.second << std::endl;
}

std::string Vertex::GetKey() const {
  return key;
}

std::string Vertex::GetName() const {
  return name;
}

std::string Vertex::GetCity() const {
  return city;
}

std::pair<double, double> Vertex::GetCoords() const {
  return coords;
}

Edge::Edge() {}

Edge::Edge(const std::string& csv) {
  std::vector<std::string> parsed;
  std::stringstream ss(csv);
  while (ss.good()) {
    std::string substring;
    std::getline(ss, substring, ',');
    parsed.push_back(substring);
  }
  source = parsed[2];
  dest = parsed[4];
  key = source + '-' + dest;
  //std::cout << source << ", " << dest << ", " << key << std::endl;
  weight = 0;
}

std::string Edge::GetKey() const {
  return key;
}

std::string Edge::GetSource() const {
  return source;
}

std::string Edge::GetDest() const {
  return dest;
}

double Edge::GetWeight() const {
  return weight;
}

void Edge::SetWeight(double new_weight) {
  weight = new_weight;
}

Graph::Graph(const std::string& vertex_file, const std::string& edge_file) {
  std::ifstream vfilestream(vertex_file);
  std::string line;
  while (std::getline(vfilestream, line)) {
    // TODO: check for null keys. if key is null, disregard and do not add to vertices map
    Vertex v(line);
    vertices[v.GetKey()] = v;
  }
  line.clear();
  std::ifstream efilestream(edge_file);
  while (std::getline(efilestream, line)) {
    // maybe TODO? check for null/abnormal edge keys, doesn't seem to be an issue from looking at dataset
    // TODO: check for repeat edges (same source/destination), skip repeat edges (avoid repeats in adjacency list)
    Edge e(line);
    edges[e.GetKey()] = e;
    adj_list[e.GetSource()].push_back(e.GetDest());
    // TODO: set edge weight to distance
  }
  std::cout << vertices.size() << ", " << edges.size() << ", " << adj_list.size() << std::endl;
}