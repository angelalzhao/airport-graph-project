#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <queue>
#include <limits>
#include <algorithm>

#include "graph.h"

/**
* Default vertex constructor
*/
Vertex::Vertex() {}

/**
* Constructs a vertex
* @param csv the line of a file representing a vertex
*/
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
  // Key should be 5 characters (three character airport code and quotation marks)
  if (key.length() != 5) {
    throw std::invalid_argument("Invalid key");
  }
  // Remove quotation marks
  key = key.substr(1, 3);
  name = parsed[1];
  // Name, city, country must all be at lease 3 characters with quotation marks
  if (name.length() < 3 || parsed[2].length() < 3 || parsed[3].length() < 3) {
    throw std::invalid_argument("Invalid city or name");
  }
  // Removing quotation marks and formating location as City, Country
  name = name.substr(1, name.length() - 2);
  city = parsed[2].substr(1, parsed[2].length() - 2) + ", " + parsed[3].substr(1, parsed[3].length() - 2);
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

/**
* Default Edge Constructor
*/
Edge::Edge() {}

/**
* Constructs an edge
* @param csv line of a file representing an edge
*/
Edge::Edge(const std::string& csv) {
  std::vector<std::string> parsed;
  std::stringstream ss(csv);
  while (ss.good()) {
    std::string substring;
    std::getline(ss, substring, ',');
    parsed.push_back(substring);
  }
  // IATA code of source airport
  source = parsed[2];
  // IATA code of destination airport
  dest = parsed[4];
  key = source + '-' + dest;
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

/**
* Constructs a graph from a file of vertices and a file of edges
* @param vertex_file file of vertices
* @param edge_file file of edges
*/

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
    Edge e(line);
    // checking if either endpoints aren't in list of vertices, or that an edge with the same source/destination already exists
    if (!vertices.count(e.GetSource()) || !vertices.count(e.GetDest()) || edges.count(e.GetKey())) {
      continue;
    }
    Vertex source = vertices[e.GetSource()];
    Vertex dest = vertices[e.GetDest()];
    // Set edge weight to distance between source and destination (according to latitude/longitude)
    e.SetWeight(Distance(source.GetCoords(), dest.GetCoords()));
    edges[e.GetKey()] = e;
    adj_list[e.GetSource()].push_back(e.GetDest());
  }
}

unsigned Graph::GetNumVertices() const {
  return vertices.size();
}

unsigned Graph::GetNumEdges() const {
  return edges.size();
}

Vertex Graph::GetVertex(const std::string& code) {
  return vertices[code];
}

std::vector<std::string> Graph::GetDestinations(const std::string& source) {
  if (!adj_list.count(source)) return {};
  return adj_list.at(source);
}

bool Graph::VertexExists(const std::string& key) {
  return vertices.count(key) > 0;
}

bool Graph::EdgeExists(const std::string& source, const std::string& dest) {
  std::string key = source + '-' + dest;
  return edges.count(key) > 0;
}

void Graph::SetEdgeWeight(const std::string& source, const std::string& dest, double weight) {
  std::string key = source + "-" + dest;
  if (edges.count(key)) edges[key].SetWeight(weight);
}

/**
* Creates a breadth-first traversal for the graph
* @return a vector of keys in the order nodes were visited
*/
std::vector<std::string> Graph::BFS() {
  // Stores the keys of all visited nodes
  std::unordered_set<std::string> visited;
  // Stores the keys of all node keys in order visited
  std::vector<std::string> v;
  for (const auto& vertex : vertices) {
    // Checking for connected components
    // If vertex has already been visited, do nothing as it belongs to a connected component that has already been explored
    if (visited.count(vertex.first)) continue;
    BFS(vertex.first, v, visited);
  }
  return v;
}

/**
* Creates a breadth-first traversal for the graph starting at a given node
* @param start the start point of the BFS
* @return a vector of keys in the order nodes were visited
*/
std::vector<std::string> Graph::BFS(const std::string& start) {
  std::vector<std::string> v;
  std::unordered_set<std::string> visited;
  BFS(start, v, visited);
  return v;
}

/**
* Helper function for the BFS traversal
* @param start the start point of the BFS
* @param v the vector of keys in the order nodes were visted
* @param visted the set of keys that have already been visited
*/
void Graph::BFS(const std::string& start, std::vector<std::string>& v, std::unordered_set<std::string>& visited) {
  std::queue<std::string> q;
  q.push(start);
  visited.insert(start);
  while (!q.empty()) {
    std::string curr = q.front();
    q.pop();
    v.push_back(curr);
    if (!adj_list.count(curr)) continue;
    // Visit all unvisited neighbors of curr and push to queue
    for (const auto& neighbor : adj_list.at(curr)) {
      // key format: "[source vertex key]-[destination vertex key]""
      std::string key = curr + "-" + neighbor;
      if (visited.count(neighbor)) continue;
      visited.insert(neighbor);
      q.push(neighbor);
    }
  }
}

/**
* Uses Dijkstras algorithm to find the shortest path through a graph
* @param start the starting point of the path
* @param end the endpoint of the path
* @return a pair of a string representing the shortest path and the distance of the shortest path
*/
std::pair<std::string, double> Graph::Dijkstras(const std::string& start, const std::string& end) {
  // reference: https://courses.grainger.illinois.edu/cs225/fa2020/resources/dijkstra/
  const double INF = std::numeric_limits<double>::max();
  // Using (ordered) set to represent priority queue
  std::set<std::pair<double, std::string>> pq;
  // Maps node to its predecessor
  std::unordered_map<std::string, std::string> previous;
  std::unordered_set<std::string> visited;
  std::unordered_map<std::string, double> distance;

  for (const auto & v : vertices) {
    pq.insert(std::pair<double, std::string>(INF, v.first));
    distance[v.first] = INF;
  }

  // Update start distance to be 0
  distance[start] = 0;
  // To update, we remove old entry in priority queue and add the entry with the updated distance
  pq.erase(std::pair<double, std::string>(INF, start));
  pq.insert(std::pair<double, std::string>(0, start));

  while (!pq.empty() && (*pq.begin()).second != end) {
    auto curr = *pq.begin();
    pq.erase(curr);
    visited.insert(curr.second);
    if (!adj_list.count(curr.second)) continue;
    for (const auto& neighbor : adj_list.at(curr.second)) {
      if (visited.count(neighbor)) continue;
      std::string edge_key = curr.second + "-" + neighbor;
      double new_dist = curr.first + edges.at(edge_key).GetWeight();
      if (new_dist < distance.at(neighbor)) {
        previous[neighbor] = curr.second;
        double prev_dist = distance.at(neighbor);
        distance[neighbor] = new_dist;
        pq.erase(std::pair<double, std::string>(prev_dist, neighbor));
        pq.insert(std::pair<double, std::string>(new_dist, neighbor));
      }
    }
    
  }
  std::string path_string;
  if (distance.at(end) == INF) {
    path_string = "No path found";
  }
  // Reconstructing the path from previous
  std::string curr = end;
  std::vector<std::string> path;
  while (previous.count(curr)) {
    curr = previous.at(curr);
    path.push_back(curr);
  }
  std::reverse(path.begin(), path.end());
  
  for (std::string node : path) {
    path_string += node;
    path_string += " -> ";
  }
  if (path_string != "No path found") {
    path_string += end;
  }

  return std::make_pair(path_string, distance.at(end));
}

/**
* PageRank algorithm to calculate the popularity of airports in the graph, based on flight legs
* @return a map of each airport key to its rank and sorted list of pagerank score/airport code pairs from highest score to lowest
*/
std::pair<std::unordered_map<std::string, double>, std::vector<std::pair<double, std::string>>> Graph::PageRank() {
  // reference: https://courses.cs.washington.edu/courses/cse373/17au/project3/project3-3.html
  double epsilon = 0.000005;
  double decay = 0.85;
  bool converged = false;
  std::unordered_map<std::string, double> ranking;
  std::unordered_map<std::string, double> previous;
  for (const auto & v : vertices) {
    ranking.insert(std::make_pair(v.first, 1.0 / vertices.size()));
  }
  while (!converged) {
    double no_outgoing = 0;
    previous = ranking;
    for (auto & r : ranking) {
      r.second = 0;
    }
    for (const auto & r : ranking) {
      // Keeping track of cumulative old page rank from nodes with nodes with no outgoing edges
      if (!adj_list.count(r.first)) {
        no_outgoing += previous[r.first];
        continue;
      }
      for (const std::string & v : adj_list[r.first]) {
        int connections = adj_list[r.first].size();
        ranking[v] += decay * previous[r.first] / connections;
      }
    }
    for (auto & r : ranking) {
      // Adding d * cumulative old rank from nodes without outgoing edges / N to all vertices 
      r.second += decay * no_outgoing / vertices.size();
      r.second += (1.0 - decay) / vertices.size();
    }
    converged = true;
    for (const auto & r : ranking) {
      if (abs(r.second - previous[r.first]) > epsilon) {
        converged = false;
        break;
      }
    }
  }

  std::vector<std::pair<double, std::string>> sorted;
  for (const auto & elem : ranking) {
    sorted.push_back(std::make_pair(elem.second, elem.first));
  }
  std::sort(sorted.begin(), sorted.end(), std::greater<>());

  return std::make_pair(ranking, sorted);
}