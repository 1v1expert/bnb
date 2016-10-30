#ifndef BNB_INPUT_HPP
#define BNB_INPUT_HPP

#include <vector>
#include <tuple>

#include "Stock.hpp"

namespace bnb
{
    using std::vector;
    using std::tuple;

    struct Input
    {
        double budget;
        vector<tuple<Stock, uint32_t>> stocks;
    };
}

#endif //BNB_INPUT_HPP

