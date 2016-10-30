#ifndef BNB_INPUTASSERT_HPP
#define BNB_INPUTASSERT_HPP

#include <iostream>
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <algorithm>

#include "Input.hpp"
#include "Output.hpp"
#include "Stock.hpp"

namespace bnb
{
    using std::string;
    using std::map;
    using std::tuple;
    using std::cerr;
    using std::endl;
    using std::get;
    using std::vector;
    using std::all_of;

    struct DataAssert
    {
        static map<string, tuple<double, double, string>> rules;

        static bool Valid(const Input& data, bool verbose = true)
        {
            vector<bool> results;

            results.emplace_back(CheckInvalid(data.budget, "budget", verbose));
            for (const auto& el: data.stocks)
            {
                results.emplace_back(CheckInvalid(get<0>(el).price, "stock_price", verbose));
                results.emplace_back(CheckInvalid(get<0>(el).probability, "stock_probability", verbose));
                results.emplace_back(CheckInvalid(get<0>(el).incoming, "incoming", verbose));
                results.emplace_back(CheckInvalid(get<1>(el), "stocks_count", verbose));
            }

            return all_of(results.cbegin(), results.cend(), [](const auto& b) { return !b; });
        }

        static bool Valid(const Output& data, bool verbose = true)
        {
            vector<bool> results;

            results.emplace_back(CheckInvalid(data.max_incoming, "max_incoming", verbose));
            results.emplace_back(CheckInvalid(data.stocks_price, "stocks_price", verbose));
            for (const auto& el: data.stocks)
            {
                results.emplace_back(CheckInvalid(get<0>(el).price, "stock_price", verbose));
                results.emplace_back(CheckInvalid(get<0>(el).probability, "stock_probability", verbose));
                results.emplace_back(CheckInvalid(get<0>(el).incoming, "incoming", verbose));
                results.emplace_back(CheckInvalid(get<1>(el), "stocks_count", verbose));
            }

            return all_of(results.cbegin(), results.cend(), [](const auto& b) { return !b; });
        }

    private:
        template<typename T>
        static bool CheckInvalid(const T& val, string name, bool verbose)
        {
            if (val < get<0>(rules[name]) || val > get<1>(rules[name]))
            {
                if (verbose)
                    cerr << "Data assertion error: " << get<2>(rules["name"])
                         << ". It must be "
                         << get<0>(rules[name]) << " <= " << val << "<=" << get<1>(rules[name]) << endl;
                return true;
            }
            return false;
        }
    };

    map<string, tuple<double, double, string>> DataAssert::rules =
    {
        {"budget",              make_tuple(0., 10000000., "Invalid budget")},
        {"max_incoming",        make_tuple(0., 10000000., "Invalid maximum stocks incoming")},
        {"stocks_price",        make_tuple(1., 10000000., "Invalid all stocks price")},
        {"stocks_count",        make_tuple(1., 10000000., "Invalid stocks count")},
        {"stock_price",         make_tuple(1., 10000000., "Invalid stock price")},
        {"stock_probability",   make_tuple(0., 1.,        "Invalid stock incoming probability")},
        {"incoming",            make_tuple(1., 10000000., "Invalid stock incoming")},
    };
}

#endif //BNB_INPUTASSERT_HPP
