#ifndef BNB_DESERIALIZER_HPP
#define BNB_DESERIALIZER_HPP

#include <istream>
#include <tuple>
#include <json/json.hpp>

#include "../Data/Stock.hpp"

namespace bnb
{
    using json = nlohmann::json;
    using std::istream;
    using std::tuple;

    using std::make_tuple;

    struct JsonDeserializer
    {
        template<typename InputData>
        InputData operator()(istream& stream)
        {
            auto data = json::parse(stream);

            vector<tuple<Stock, uint32_t>> stocks;
            for (auto& stock : data["stocks"])
                stocks.emplace_back(make_tuple<Stock, uint32_t>({stock["name"],
                                                                stock["price"],
                                                                stock["probability"],
                                                                stock["incoming"]},
                                                               stock["count"]));
            return {data["budget"].get<double>(), stocks};
        }
    };
}

#endif //BNB_DESERIALIZER_HPP
