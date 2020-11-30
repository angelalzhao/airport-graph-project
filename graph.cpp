#include "graph.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
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
  // stod exception caused by extra commas (usually in airport name), disregard these airports for now
  coords.first = std::stod(parsed[6]);
  coords.second = std::stod(parsed[7]);
  key = parsed[4];
  if (key.length() != 5) {
    throw std::invalid_argument("Invalid key");
  }
  key = key.substr(1, 3);
  name = parsed[1];
  // likely not an issue
  if (name.length() < 3 || parsed[2].length() < 3 || parsed[3].length() < 3) {
    throw std::invalid_argument("Invalid city or name");
  }
  name = name.substr(1, name.length() - 2);
  city = parsed[2].substr(1, parsed[2].length() - 2) + ", " + parsed[3].substr(1, parsed[3].length() - 2);
  // std::cout << key << ", " << name << ", " << city << ", " << coords.first << ", " << coords.second << std::endl;
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
    // if something went wrong with vertex creation, don't include in vertices map
    try {
      Vertex v(line);
      vertices[v.GetKey()] = v;
    } catch (const std::invalid_argument& ia) {
      // Do nothing
    }
    
  }
  line.clear();
  std::ifstream efilestream(edge_file);
  while (std::getline(efilestream, line)) {
    // maybe TODO? check for null/abnormal edge keys, doesn't seem to be an issue from looking at dataset
    Edge e(line);
    // checking if either endpoints aren't in list of vertices, or that an edge with the same source/destination already exists
    if (!vertices.count(e.GetSource()) || !vertices.count(e.GetDest()) || edges.count(e.GetKey())) {
      continue;
    }
    edges[e.GetKey()] = e;
    adj_list[e.GetSource()].push_back(e.GetDest());
    // TODO: set edge weight to distance
  }
  std::cout << vertices.size() << ", " << edges.size() << ", " << adj_list.size() << std::endl;
}

unsigned Graph::GetNumVertices() const {
  return vertices.size();
}

unsigned Graph::GetNumEdges() const {
  return edges.size();
}

std::vector<std::string> Graph::GetDestinations(const std::string& source) {
  if (!adj_list.count(source)) return {};
  return adj_list.at(source);
}