#ifndef GRAPH_H_
#define GRAPH_H_

#include <utility>
#include <vector>

#include "dsu.h"

template <typename Edge>
struct Graph {
  int n;
  std::vector<Edge> edges;
  std::vector<std::vector<int>> go;
  Graph(int n_) : n(n_) { go.resize(n); }
  virtual void AddEdge(Edge e) = 0;
};

template <typename Edge>
struct DirectedGraph : public Graph<Edge> {
  using edge_t = Edge;
  using is_directed = std::bool_constant<true>;
  DirectedGraph(int n_) : Graph<Edge>(n_) {}
  void AddEdge(Edge e) {
    assert(e.u >= 0 && e.u < this->n && e.v >= 0 && e.v < this->n);
    this->edges.push_back(e);
    this->go[e.u].push_back(this->edges.size() - 1);
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

template <typename Edge>
struct UndirectedGraph : public Graph<Edge> {
  using edge_t = Edge;
  using is_directed = std::bool_constant<false>;
  UndirectedGraph(int n_) : Graph<Edge>(n_) {}
  void AddEdge(Edge e) {
    assert(e.u >= 0 && e.u < this->n && e.v >= 0 && e.v < this->n);
    this->edges.push_back(e);
    this->go[e.u].push_back(this->edges.size() - 1);
    this->go[e.v].push_back(this->edges.size() - 1);
  }
};

#endif  // GRAPH_H_