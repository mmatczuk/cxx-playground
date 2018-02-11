#include <iostream>
#include <fstream>

#include <graph.h>

int main(int argc, char **argv)
{
    // const int nodes = 50;
    // const double density = 0.5;
    // const double min_distance = 1;
    // const double max_distance = 10;
    // auto g = new_random_graph(nodes, density, min_distance, max_distance);

    if (argc < 2)
    {
        std::cerr << "Error: missing file argument" << std::endl;
        return 1;
    }

    std::fstream f;
    f.open(argv[1], std::fstream::in);
    if (!f.is_open())
    {
        std::cerr << "Error: file '" << argv[1] << "' could not be opened" << std::endl;
        return 1;
    }
    auto g = graph::read_graph(f);
    // std::cout << *g << std::endl;
    f.close();

    double cost;
    auto t = g->min_spanning_tree(cost);
    if (t == nullptr)
    {
        std::cerr << "Error: MST not found" << std::endl;
        return 1;
    }
    else
    {
        std::cout << *t << std::endl;
        std::cout << cost << std::endl;
    }

    return 0;
}
