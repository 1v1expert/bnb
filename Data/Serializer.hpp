#ifndef BNB_SERIALIZER_HPP
#define BNB_SERIALIZER_HPP

#include <ostream>
#include <tuple>
#include <json/json.hpp>

#include "../Data/Stock.hpp"

namespace bnb
{
    using json = nlohmann::json;
    using std::ostream;
    using std::tuple;
    using std::get;

    using std::make_tuple;

    struct JsonSerializer
    {
        template<typename OutputData>
        void operator()(ostream& stream, OutputData& output)
        {
            json j;
            j["max_incoming"] = output.max_incoming;
            j["stocks_price"] = output.stocks_price;
            for (auto& stock : output.stocks)
            {
                json stock_j;
                stock_j["name"] = get<0>(stock).name;
                stock_j["count"] = get<1>(stock);
                j["stocks"].push_back(stock_j);
            }
            stream << j;
        }
    };
}

#endif //BNB_SERIALIZER_HPP
