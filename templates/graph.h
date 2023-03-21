#ifndef GRAPH_H_
#define GRAPH_H_

#include <utility>
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
    assert(e.u >= 0 && e.u < n && e.v >= 0 && e.v < n);
    edges.push_back(e);
    go[e.u].push_back(edges.size() - 1);
  }
};

template <typename Edge>
struct FlowGraph : public DirectedGraph<Edge> {
  using flow_t = decltype(std::declval<Edge>().cap);
  FlowGraph(int n_) : DirectedGraph<Edge>(n_) {}
  static const flow_t eps = (flow_t)1e-7;
  void AddFlowEdge(Edge e) {
    this->AddEdge(e);
    std::swap(e.u, e.v);
    e.cap = 0;
    this->AddEdge(e);
  }
  void AddFlowEdgeWithCost(Edge e) {
    this->AddEdge(e);
    std::swap(e.u, e.v);
    e.cap = 0;
    e.cost *= -1;
    this->AddEdge(e);
  }
};

#endif  // GRAPH_H_