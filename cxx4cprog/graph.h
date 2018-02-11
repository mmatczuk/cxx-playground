#ifndef TASK_02_GRAPH_H_
#define TASK_02_GRAPH_H_

#include <istream>
#include <ostream>
#include <map>
#include <memory>
#include <vector>

namespace graph
{

// Node specifies a graph node.
template <typename T>
class Node
{
public:
  explicit Node(int id, const T &t) : id(id), v(t){};
  Node() = delete;

  template <typename T1>
  friend std::ostream &operator<<(std::ostream &out, const Node<T1> &n);

  void add_edge(int to, double weight);
  const std::map<int, double> &adjunct();

  const T &value()
  {
    return this->v;
  }

private:
  int id;
  T v;
  std::map<int, double> e;
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const Node<T> &n)
{
  out << n.id << " - > [";
  for (auto e : n.e)
  {
    out << " " << e.first << "(" << e.second << ")";
  }
  out << " ]";
  return out;
}

// Graph represents a graph of T, every node has an int id inside a graph.
template <typename T>
class Graph
{
public:
  template <typename T1>
  friend std::ostream &operator<<(std::ostream &out, const Graph<T1> &g);

  int add_node(const T &t);
  int add_edge(int from, int to, double weight = 0);
  std::unique_ptr<Graph<T>> min_spanning_tree(double &cost);

private:
  std::vector<Node<T>> v;
};

// read_graph reads a graph from an input stream. The format is:
//
// <number of nodes>
// <from> <to> <weight>
// <from> <to> <weight>
// <from> <to> <weight>
// ...
std::unique_ptr<Graph<int>> read_graph(std::istream &in);

// new_random_graph generates a random graph with a given number of nodes and
// density.
std::unique_ptr<graph::Graph<int>> new_random_graph(int nodes, double density, double min_distance, double max_distance);

template <typename T>
std::ostream &operator<<(std::ostream &out, const Graph<T> &g)
{
  for (auto n : g.v)
  {
    out << n << std::endl;
  }
  return out;
}
}

#endif // TASK_02_GRAPH_H_
