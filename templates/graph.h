#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

template <typename Edge>
struct DirectedGraph {
  using edge_t = Edge;
  using is_directed = std::bool_constant<true>;
  int n;
  std::vector<Edge> edges;
  std::vector<std::vector<int>> go;
  DirectedGraph(int n_) : n(n_) { go.resize(n); }
  void AddEdge(Edge e) {
    edges.push_back(e);
    go[e.u].push_back(edges.size() - 1);
  }
};

#endif  // GRAPH_H_