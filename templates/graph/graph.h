#ifndef GRAPH_H_
#define GRAPH_H_

#include <cassert>
#include <string>
#include <utility>
#include <vector>

struct EdgeBase {
  int u, v;
  operator std::string() {
    return "Edge(" + std::to_string(u) + ", " + std::to_string(v) + ")";
  }
};

template <typename Graph>
struct GraphNeighborsRange {
  struct Iterator {
    Graph* g;
    int u, id;
    bool is_directed;
    Iterator(Graph* g_, int u_, int id_) : g(g_), u(u_), id(id_) {
      is_directed = g->IsDirected();
    }
    Iterator& operator++() {
      id++;
      return *this;
    }
    bool operator!=(const Iterator& other) { return id != other.id; }
    int operator*() {
      assert(id < g->go[u].size());
      auto& e = g->edges[g->go[u][id]];
      if (is_directed) {
        return e.v;
      } else {
        return u ^ e.u ^ e.v;
      }
    }
  };
  Graph* g;
  int u;
  GraphNeighborsRange(Graph* g_, int u_) : g(g_), u(u_) {
    assert(0 <= u && u < g->n);
  }
  Iterator begin() { return Iterator(g, u, 0); }
  Iterator end() { return Iterator(g, u, g->go[u].size()); }
};

template <typename Graph>
struct GraphEdgesRange {
  struct Iterator {
    Graph* g;
    int u, id;
    Iterator(Graph* g_, int u_, int id_) : g(g_), u(u_), id(id_) {}
    Iterator& operator++() {
      id++;
      return *this;
    }
    bool operator!=(const Iterator& other) { return id != other.id; }
    typename Graph::edge_t& operator*() {
      assert(id < g->go[u].size());
      return g->edges[g->go[u][id]];
    }
  };
  Graph* g;
  int u;
  GraphEdgesRange(Graph* g_, int u_) : g(g_), u(u_) {
    assert(0 <= u && u < g->n);
  }
  Iterator begin() { return Iterator(g, u, 0); }
  Iterator end() { return Iterator(g, u, g->go[u].size()); }
};

template <typename Edge>
struct Graph {
  using edge_t = Edge;
  int n;
  std::vector<Edge> edges;
  std::vector<std::vector<int>> go;
  Graph(int n_) : n(n_) { go.resize(n); }
  virtual bool IsDirected() const { assert(0); }
  virtual void AddEdge(Edge e) = 0;
  GraphNeighborsRange<Graph> Neighbors(int u) {
    return GraphNeighborsRange<Graph>(this, u);
  }
  GraphEdgesRange<Graph> Edges(int u) {
    return GraphEdgesRange<Graph>(this, u);
  }
};

template <typename Edge>
struct UndirectedGraph : public Graph<Edge> {
  UndirectedGraph(int n_) : Graph<Edge>(n_) {}
  virtual bool IsDirected() const { return false; }
  virtual void AddEdge(Edge e) {
    assert(e.u >= 0 && e.u < this->n && e.v >= 0 && e.v < this->n);
    this->edges.push_back(e);
    this->go[e.u].push_back(this->edges.size() - 1);
    this->go[e.v].push_back(this->edges.size() - 1);
  }
};

template <typename Edge>
struct DirectedGraph : public Graph<Edge> {
  DirectedGraph(int n_) : Graph<Edge>(n_) {}
  virtual bool IsDirected() const { return true; }
  virtual void AddEdge(Edge e) {
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
  virtual void AddEdge(Edge) { assert(false); }
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

#endif  // GRAPH_H_