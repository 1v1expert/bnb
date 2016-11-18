#ifndef BNB_GRAPH_HPP
#define BNB_GRAPH_HPP

#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <memory>
#include <bandit/bandit.h>

#include "../Data/Input.hpp"

namespace bnb
{
    using namespace bandit;

    using std::vector;
    using std::list;
    using std::tuple;
    using std::unique_ptr;
    using std::string;

    using std::move;
    using std::max_element;
    using std::get;
    using std::make_tuple;
    using std::make_unique;
    using std::find_if;
    using std::accumulate;

    template<typename NodeType>
    struct GraphNode
    {
        vector<GraphNode<NodeType>*> parents;
        NodeType node;
        vector<GraphNode<NodeType>*> childs;
    };

    struct Graph
    {
        using Node = GraphNode<Stock>;
        using SolutionTuple = tuple<list<Node*>, double, double>;

        vector<Node*> layers;

        Graph(const Input& input) :
                bnb_incoming(0.),
                bnb_probability(0.)
        {
            auto max_count_stock = max_element(input.stocks.cbegin(), input.stocks.cend(),
                                               [](auto& lhs, auto& rhs) { return get<1>(lhs) < get<1>(rhs); });
            if (max_count_stock == input.stocks.cend()) return;

            decltype(Node::parents) parents;
            for (auto i = 0; i < get<1>(*max_count_stock); ++i)
            {
                if (parents.size() > 0)
                {
                    decltype(parents) childs;
                    for (auto& stock: input.stocks)
                        if (get<1>(stock) > i)
                        {
                            allocated_layers.push_back(make_unique<Node>(Node{{}, get<0>(stock), {}}));
                            childs.push_back(allocated_layers.back().get());
                        }

                    for (auto& parent: parents)
                        for (auto& child: childs)
                            parent->childs.push_back(child);

                    for (auto& child: childs)
                        for (auto& parent: parents)
                            child->parents.push_back(parent);

                    parents.clear();
                    for (auto& child: childs)
                        parents.push_back(child);
                }
                else
                {
                    for (auto& stock: input.stocks)
                        if (get<1>(stock) > i)
                        {
                            allocated_layers.push_back(make_unique<Node>(Node{{}, get<0>(stock), {}}));
                            parents.push_back(allocated_layers.back().get());
                            layers.push_back(allocated_layers.back().get());
                        }
                }
            }
        }

        uint32_t CalculateNodeDepth(string name)
        {
            uint32_t depth = 0u;
            auto node = find_if(layers.cbegin(), layers.cend(),
                                [&name](auto& node) { return node->node.name == name; });

            if (node != layers.cend())
                CalculateNodeDepth_recursive(name, *node, ++depth);

            return depth;
        }

        template<typename MaximizeFunc>
        SolutionTuple CalculateBnbPath(double bound,
                                       MaximizeFunc maximize_func)
        {
            list<Node*> temp_solution;
            for (auto& node: layers)
            {
                temp_solution.emplace_back(node);

                if (CalculatePriceBound(temp_solution) <= bound)
                {
                    auto opt = CalculateOptimizeBound(temp_solution);
                    if (maximize_func(bnb_probability, bnb_incoming) < maximize_func(get<0>(opt),
                                                                                     get<1>(opt)))
                    {
                        bnb_graph_path = temp_solution;
                        bnb_probability = get<0>(opt);
                        bnb_incoming = get<1>(opt);
                    }

                    CalculateBnbPath_recursive(bound, maximize_func, temp_solution, node);
                }

                temp_solution.remove(node);
            }

            return make_tuple(bnb_graph_path, bnb_probability, bnb_incoming);
        }

        double CalculatePriceBound(list<Node*> current_path)
        {
            return accumulate(current_path.cbegin(), current_path.cend(), 0.,
                              [&](auto& sum, auto& el) { return sum += el->node.price; });
        }

    private:
        vector<unique_ptr<Node>> allocated_layers;
        list<Node*> bnb_graph_path;
        double bnb_probability;
        double bnb_incoming;

        void CalculateNodeDepth_recursive(string& name, Node* node, uint32_t& depth)
        {
            auto new_node = find_if(node->childs.cbegin(), node->childs.cend(),
                                    [&name](auto& node) { return node->node.name == name; });
            if (new_node != node->childs.cend())
            {
                CalculateNodeDepth_recursive(name, *new_node, ++depth);
            }
        }

        tuple<double, double> CalculateOptimizeBound(list<Node*> current_path)
        {
            return accumulate(current_path.cbegin(), current_path.cend(), tuple<double, double>(0., 0.),
                              [&](auto& tup, auto& el)
                              {
                                  get<0>(tup) = (get<0>(tup) + el->node.probability) -
                                                (get<0>(tup) * el->node.probability);
                                  get<1>(tup) += el->node.incoming;
                                    return tup;
                              });
        }

        template<typename MaximizeFunc>
        void CalculateBnbPath_recursive(const double& bound,
                                        MaximizeFunc& maximize_func,
                                        list<Node*>& temp_solution,
                                        Node* current_node)
        {
            for (auto& node: current_node->childs)
            {
                temp_solution.emplace_back(node);

                if (CalculatePriceBound(temp_solution) <= bound)
                {
                    auto opt = CalculateOptimizeBound(temp_solution);
                    if (maximize_func(bnb_probability, bnb_incoming) < maximize_func(get<0>(opt),
                                                                                     get<1>(opt)))
                    {
                        bnb_graph_path = temp_solution;
                        bnb_probability = get<0>(opt);
                        bnb_incoming = get<1>(opt);
                    }

                    CalculateBnbPath_recursive(bound, maximize_func, temp_solution, node);
                }

                temp_solution.remove(node);
            }
        }
    };


    go_bandit([]() {
        describe("Graph generation test", []() {
            Input input1 = {500000.,
                           {
                                   make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                                   make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                                   make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 50)
                           }};
            Input input2 = {54874.,
                           {
                                   make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 0),
                                   make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 1)
                           }};
            Input input3 = {50016000.,
                            {
                                    make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 3),
                                    make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 567),
                                    make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 1),
                                    make_tuple<Stock, uint32_t>({"stock_4", 91., 0.63f, 148.}, 11)
                            }};
            Input input4 = {100000.,
                            {
                                    make_tuple<Stock, uint32_t>({"stock_1", 1400., 0.27f, 3600.}, 5),
                                    make_tuple<Stock, uint32_t>({"stock_2", 8000., 0.68f, 8533.}, 8),
                                    make_tuple<Stock, uint32_t>({"stock_3", 1000., 0.87f, 15000.}, 3),
                                    make_tuple<Stock, uint32_t>({"stock_4", 12000., 0.49f, 19000.}, 4),
                                    make_tuple<Stock, uint32_t>({"stock_5", 3100., 0.74f, 3200.}, 7)
                            }};

            Graph graph1(input1);
            Graph graph2(input2);
            Graph graph3(input3);
            Graph graph4(input4);

            it("Check graph1 count", [&]() {
                AssertThat(graph1.CalculateNodeDepth("stock_1"), Equals(200));
                AssertThat(graph1.CalculateNodeDepth("stock_2"), Equals(100));
                AssertThat(graph1.CalculateNodeDepth("stock_3"), Equals(50));
            });

            it("Check graph2 count", [&]() {
                AssertThat(graph2.CalculateNodeDepth("stock_1"), Equals(0));
                AssertThat(graph2.CalculateNodeDepth("stock_3"), Equals(1));
            });

            it("Check graph3 count", [&]() {
                AssertThat(graph3.CalculateNodeDepth("stock_1"), Equals(3));
                AssertThat(graph3.CalculateNodeDepth("stock_2"), Equals(567));
                AssertThat(graph3.CalculateNodeDepth("stock_3"), Equals(1));
                AssertThat(graph3.CalculateNodeDepth("stock_4"), Equals(11));
            });

            it("Check graph4 count", [&]() {
                AssertThat(graph4.CalculateNodeDepth("stock_1"), Equals(5));
                AssertThat(graph4.CalculateNodeDepth("stock_2"), Equals(8));
                AssertThat(graph4.CalculateNodeDepth("stock_3"), Equals(3));
                AssertThat(graph4.CalculateNodeDepth("stock_4"), Equals(4));
                AssertThat(graph4.CalculateNodeDepth("stock_5"), Equals(7));
            });
        });

        describe("Graph bnb path test", [](){
            Input input = {100000.,
                           {
                               make_tuple<Stock, uint32_t>({"stock_1", 1400., 0.27f, 3600.}, 5),
                               make_tuple<Stock, uint32_t>({"stock_2", 8000., 0.68f, 8533.}, 8),
                               make_tuple<Stock, uint32_t>({"stock_3", 1000., 0.87f, 8000.}, 3),
                               make_tuple<Stock, uint32_t>({"stock_4", 5000., 0.49f, 1000.}, 4),
                               make_tuple<Stock, uint32_t>({"stock_5", 3100., 0.74f, 3200.}, 7)
                           }};

            Graph graph(input);

            it("Check graph bnb path", [&]() {
                auto result = graph.CalculateBnbPath(input.budget, [](auto& p, auto& i) { return i; });
                std::cout << get<0>(result).size() << '\t'
                          << graph.CalculatePriceBound(get<0>(result)) << '\t'
                          << get<1>(result) << '\t'
                          << get<2>(result)
                          << std::endl;
            });
        });
    });
}

#endif //BNB_GRAPH_HPP
