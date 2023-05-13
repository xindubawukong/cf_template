#ifndef GRAPH_H_
#define GRAPH_H_

#include <cassert>
#include <utility>
#include <vector>

template <typename Graph>
struct GraphNeighborsRange {
  struct Iterator {
    const Graph& g;
    int u, id;
    Iterator(const Graph& g_, int u_, int id_) : g(g_), u(u_), id(id_) {}
    Iterator& operator++() {
      id++;
      return *this;
    }
    bool operator!=(const Iterator& other) { return id != other.id; }
    int operator*() {
      assert(id < g.go[u].size());
      auto& e = g.edges[g.go[u][id]];
      return g.NeighborId(u, e);
    }
  };
  const Graph& g;
  int u;
  GraphNeighborsRange(const Graph& g_, int u_) : g(g_), u(u_) {
    assert(0 <= u && u <= g.n);
  }
  Iterator begin() { return Iterator(g, u, 0); }
  Iterator end() { return Iterator(g, u, g.go[u].size()); }
};

template <typename Graph>
struct GraphEdgesRange {
  struct Iterator {
    const Graph& g;
    int u, id;
    Iterator(const Graph& g_, int u_, int id_) : g(g_), u(u_), id(id_) {}
    Iterator& operator++() {
      id++;
      return *this;
    }
    bool operator!=(const Iterator& other) { return id != other.id; }
    const typename Graph::edge_t& operator*() {
      assert(id < g.go[u].size());
      return g.edges[g.go[u][id]];
    }
  };
  const Graph& g;
  int u;
  GraphEdgesRange(const Graph& g_, int u_) : g(g_), u(u_) {
    assert(0 <= u && u <= g.n);
  }
  Iterator begin() { return Iterator(g, u, 0); }
  Iterator end() { return Iterator(g, u, g.go[u].size()); }
};

template <typename Edge>
struct Graph {
  using edge_t = Edge;
  int n;
  std::vector<Edge> edges;
  std::vector<std::vector<int>> go;
  Graph(int n_) : n(n_) { go.resize(n); }
  virtual void AddEdge(Edge e) = 0;
  virtual GraphEdgesRange<Graph<Edge>> Edges(int u) const {
    return GraphEdgesRange(*this, u);
  }
};

template <typename Edge>
struct DirectedGraph : public Graph<Edge> {
  using is_directed = std::bool_constant<true>;
  DirectedGraph(int n_) : Graph<Edge>(n_) {}
  virtual void AddEdge(Edge e) {
    assert(e.u >= 0 && e.u < this->n && e.v >= 0 && e.v < this->n);
    this->edges.push_back(e);
    this->go[e.u].push_back(this->edges.size() - 1);
  }
  virtual int NeighborId(int u, const Edge& e) const { return e.v; }
  virtual GraphNeighborsRange<DirectedGraph<Edge>> Neighbors(int u) const {
    return GraphNeighborsRange<DirectedGraph<Edge>>(*this, u);
  }
};

template <typename Edge>
struct FlowGraph : public DirectedGraph<Edge> {
  using flow_t = decltype(std::declval<Edge>().cap);
  FlowGraph(int n_) : DirectedGraph<Edge>(n_) {}
  static const flow_t eps = (flow_t)1e-7;
  virtual void AddEdge(Edge e) { assert(false); }
  void AddFlowEdge(Edge e) {
    DirectedGraph<Edge>::AddEdge(e);
    std::swap(e.u, e.v);
    e.cap = 0;
    DirectedGraph<Edge>::AddEdge(e);
  }
  void AddFlowEdgeWithCost(Edge e) {
    DirectedGraph<Edge>::AddEdge(e);
    std::swap(e.u, e.v);
    e.cap = 0;
    e.cost *= -1;
    DirectedGraph<Edge>::AddEdge(e);
  }
};

template <typename Edge>
struct UndirectedGraph : public Graph<Edge> {
  using is_directed = std::bool_constant<false>;
  UndirectedGraph(int n_) : Graph<Edge>(n_) {}
  virtual void AddEdge(Edge e) {
    assert(e.u >= 0 && e.u < this->n && e.v >= 0 && e.v < this->n);
    this->edges.push_back(e);
    this->go[e.u].push_back(this->edges.size() - 1);
    this->go[e.v].push_back(this->edges.size() - 1);
  }
  virtual int NeighborId(int u, const Edge& e) const { return e.u ^ e.v ^ u; }
  virtual GraphNeighborsRange<UndirectedGraph<Edge>> Neighbors(int u) const {
    return GraphNeighborsRange<UndirectedGraph<Edge>>(*this, u);
  }
};

#endif  // GRAPH_H_