#ifndef BNB_BNB_HPP
#define BNB_BNB_HPP

#include <functional>
#include <tuple>
#include <vector>
#include <list>
#include <unordered_map>

#include "../Data/Input.hpp"
#include "../Data/Output.hpp"
#include "Graph.hpp"

namespace bnb
{
    using std::function;
    using std::tuple;
    using std::vector;
    using std::list;
    using std::unordered_map;

    using std::make_tuple;

    auto cost_function = [](){};
    auto bound_function = [](){};

    struct Bnb
    {
        using Node = GraphNode<Stock>;

        function<double(double, double)> max_func = [](double p, double i) { return p * i; };

        Output operator()(const Input& input)
        {
            Graph graph(input);
            auto result = graph.CalculateBnbPath(input.budget, max_func);
            return {get<2>(result),
                    graph.CalculatePriceBound(get<0>(result)),
                    get<1>(result),
                    GetStocksBySolution(get<0>(result))};
        }

        vector<tuple<Stock, uint32_t>> GetStocksBySolution(list<Node*> solution)
        {
            unordered_map<string, tuple<Stock, uint32_t>> m;
            for (auto& node: solution)
                if (m.count(node->node.name))
                    ++get<1>(m[node->node.name]);
                else
                    m[node->node.name] = make_tuple(node->node, 1);

            vector<tuple<Stock, uint32_t>> result;
            for (auto& el: m)
                result.emplace_back(el.second);

            return result;
        };
    };
}

#endif //BNB_BNB_HPP
