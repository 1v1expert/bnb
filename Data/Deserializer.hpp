#ifndef BNB_DESERIALIZER_HPP
#define BNB_DESERIALIZER_HPP

#include <string>
#include <istream>
#include <sstream>
#include <tuple>
#include <json/json.hpp>

#include "../Data/Stock.hpp"
#include "Converter.hpp"

namespace bnb
{
    using json = nlohmann::json;
    using std::istream;
    using std::tuple;

    using std::getline;
    using std::get;
    using std::make_tuple;
    using std::stringstream;

    template<typename InputData>
    struct JsonDeserializer
    {
        InputData operator()(istream& stream)
        {
            try
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
            catch (...)
            {
                return {};
            }
        }
    };

    template<typename InputData>
    struct CsvDeserializer
    {
        const char separator = ';';

        InputData operator()(istream& stream)
        {
            string line;
            InputData input;

            getline(stream, line, '\n');
            input.budget = Converter<int32_t>::Convert(line);

            while (getline(stream, line, '\n'))
            {
                stringstream str_stream(line);
                tuple<Stock, uint32_t> stock;
                string tmp;

                getline(str_stream, tmp, separator);
                get<0>(stock).name = tmp;

                getline(str_stream, tmp, separator);
                get<0>(stock).price = Converter<double>::Convert(tmp);

                getline(str_stream, tmp, separator);
                get<0>(stock).probability = Converter<float>::Convert(tmp);

                getline(str_stream, tmp, separator);
                get<0>(stock).incoming = Converter<double>::Convert(tmp);

                getline(str_stream, tmp, separator);
                get<1>(stock) = Converter<int32_t>::Convert(tmp);

                input.stocks.emplace_back(stock);
            }

            return input;
        }
    };
}

#endif //BNB_DESERIALIZER_HPP
