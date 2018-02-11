#include <graph.h>

#include <chrono>
#include <istream>
#include <cassert>
#include <limits>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <tuple>
#include <queue>

template <typename T>
void graph::Node<T>::add_edge(int to, double weight)
{
    this->e.insert({to, weight});
}

template <typename T>
const std::map<int, double> &graph::Node<T>::adjunct()
{
    return this->e;
}

// this adds int support for testing otherwise linking fails
// main.cc:(.text+0x6c): undefined reference to `graph::Graph<int>::add_node(int const&)'
template class graph::Graph<int>;

template <typename T>
int graph::Graph<T>::add_node(const T &t)
{
    auto id = this->v.size();
    this->v.push_back(graph::Node<T>(id, t));
    return id;
}

template <typename T>
int graph::Graph<T>::add_edge(int from, int to, double weight)
{
    assert(from >= 0);
    assert(from < this->v.size());
    assert(to >= 0);
    assert(to < this->v.size());

    this->v[from].add_edge(to, weight);
}

template <typename T>
std::unique_ptr<graph::Graph<T>> graph::Graph<T>::min_spanning_tree()
{
    int n = this->v.size();
    assert(n > 0);

    std::priority_queue<
        std::tuple<double, int, int>,
        std::vector<std::tuple<double, int, int>>,
        std::greater<std::tuple<double, int, int>>>
        edges;

    std::set<int> closed;

    // create new graph
    std::unique_ptr<graph::Graph<T>> g{new graph::Graph<T>()};
    // copy nodes
    for (auto node : this->v)
    {
        g->add_node(node.value());
    }
    // init edges and closed set
    for (auto && [ node, distance ] : this->v[0].adjunct())
    {
        edges.push({distance, node, 0});
    }
    closed.insert(0);

    while (!edges.empty())
    {
        // get next egde
        auto[weight, external, internal] = edges.top();
        edges.pop();

        if (closed.find(external) != closed.end())
        {
            continue;
        }

        // add edge to the tree
        g->add_edge(external, internal, weight);
        g->add_edge(internal, external, weight);
        closed.insert(external);

        for (auto && [ node, distance ] : this->v[external].adjunct())
        {
            // add only from not closed nodes
            if (closed.find(node) == closed.end())
            {
                edges.push({distance, node, external});
            }
        }
    }

    if (closed.size() != n)
    {
        return std::unique_ptr<graph::Graph<T>>(nullptr);
    }

    return g;
}

std::unique_ptr<graph::Graph<int>> graph::read_graph(std::istream &in)
{
    int n;
    in >> n;

    // create new graph
    std::unique_ptr<graph::Graph<int>> g{new graph::Graph<int>()};
    // copy nodes
    for (int i = 0; i < n; i++)
    {
        g->add_node(i);
    }

    int from, to;
    double weight;
    while (in >> from >> to >> weight)
    {
        assert(from < n);
        assert(to < n);
        g->add_edge(from, to, weight);
    }

    return g;
}

std::unique_ptr<graph::Graph<int>> new_random_graph(int nodes, double density, double min_distance, double max_distance)
{
    std::unique_ptr<graph::Graph<int>> g{new graph::Graph<int>()};

    // add nodes
    for (int i = 0; i < nodes; i++)
    {
        g->add_node(i);
    }

    unsigned const seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_real_distribution<double> should_add_edge(0, 1.0);
    std::uniform_real_distribution<double> distance(min_distance, max_distance);

    // add edges
    for (int i = 0; i < nodes; i++)
    {
        for (int j = i + 1; j < nodes; j++)
        {
            if (should_add_edge(gen) > 1 - density)
            {
                auto d = distance(gen);
                g->add_edge(i, j, d);
                g->add_edge(j, i, d);
            }
        }
    }

    return g;
}
