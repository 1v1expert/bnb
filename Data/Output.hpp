#ifndef BNB_OUTPUT_HPP
#define BNB_OUTPUT_HPP

#include <vector>
#include <tuple>

namespace bnb
{
    using std::vector;
    using std::tuple;

    struct Output
    {
        double max_incoming;
        double stocks_price;
        double probability;
        vector<tuple<Stock, uint32_t>> stocks;
    };
}

#endif //BNB_OUTPUT_HPP
