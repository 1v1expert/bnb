#include <bandit/bandit.h>

#include "../Data/Input.hpp"
#include "../Data/Output.hpp"
#include "../Data/Serializer.hpp"
#include "../Data/Deserializer.hpp"
#include "../Data/DataAssert.hpp"

namespace bnb
{
    using namespace bandit;
    using json = nlohmann::json;
    using std::numeric_limits;
    using std::vector;
    using std::stringstream;

    using std::make_tuple;
    using std::get;
    using std::find_if;

    go_bandit([](){
        describe("Input test", [](){
            Input input_data = {5000.,
                                {
                                    make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                                    make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                                    make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 50)
                                }};

            it("Check budget", [&](){
                AssertThat(input_data.budget, EqualsWithDelta(5000.,
                                                              numeric_limits<double>::epsilon()));
            });

            it("Check stocks", [&](){
                AssertThat(input_data.stocks, EqualsContainer(vector<tuple<Stock, uint32_t>>{
                        make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                        make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                        make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 50)
                }));
            });

            it("Check budget from copy", [&](){
                auto input_data_copy = input_data;
                AssertThat(input_data_copy.budget, EqualsWithDelta(5000.,
                                                                   numeric_limits<double>::epsilon()));
            });

            it("Check stocks from copy", [&](){
                auto input_data_copy = input_data;
                AssertThat(input_data_copy.stocks, EqualsContainer(vector<tuple<Stock, uint32_t>>{
                        make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                        make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                        make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 50)
                }));
            });
        });

        describe("Output test", [](){
            Output output = {546510.,
                             78415.,
                             0.48,
                             {
                                 make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                                 make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                                 make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 50)
                             }};

            it("Check max incoming", [&](){
                AssertThat(output.max_incoming, EqualsWithDelta(546510.,
                                                                numeric_limits<double>::epsilon()));
            });

            it("Check stocks price", [&](){
                AssertThat(output.stocks_price, EqualsWithDelta(78415.,
                                                                numeric_limits<double>::epsilon()));
            });

            it("Check stocks", [&](){
                AssertThat(output.stocks, EqualsContainer(vector<tuple<Stock, uint32_t>>{
                        make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                        make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                        make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 50)
                }));
            });

            it("Check max incoming copy", [&](){
                auto output_data = output;
                AssertThat(output_data.max_incoming, EqualsWithDelta(546510.,
                                                                     numeric_limits<double>::epsilon()));
            });

            it("Check stocks price copy", [&](){
                auto output_data = output;
                AssertThat(output_data.stocks_price, EqualsWithDelta(78415.,
                                                                     numeric_limits<double>::epsilon()));
            });

            it("Check stocks copy", [&](){
                auto output_data = output;
                AssertThat(output_data.stocks, EqualsContainer(vector<tuple<Stock, uint32_t>>{
                        make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                        make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                        make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 50)
                }));
            });
        });

        describe("Json serializer test", [&](){
            Output output = {546510.,
                             78415.,
                             0.48,
                             {
                                 make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                                 make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                                 make_tuple<Stock, uint32_t>({"stock_3", 91., 0.63f, 148.}, 50)
                             }};

            it("Check json serializer", [&](){
                stringstream result;
                JsonSerializer{}(result, output);

                auto j = json::parse(result);
                AssertThat(j["max_incoming"].get<double>(), EqualsWithDelta(output.max_incoming,
                                                                               numeric_limits<double>::epsilon()));
                AssertThat(j["stocks_price"].get<double>(), EqualsWithDelta(output.stocks_price,
                                                                               numeric_limits<double>::epsilon()));
                for (const auto& stock: output.stocks)
                {
                    auto j_sock = find_if(j["stocks"].cbegin(),
                                          j["stocks"].cend(),
                                          [&](const auto& j_){ return j_["name"] == get<0>(stock).name; });
                    AssertThat(j_sock, !Equals(j["stocks"].cend()));
                    AssertThat(get<0>(stock).name, Equals((*j_sock)["name"].get<string>()));
                    AssertThat(get<1>(stock), Equals((*j_sock)["count"].get<uint32_t >()));
                }
            });
        });

        describe("Csv serializer test", [&](){
            Output output = {546510.,
                             78415.,
                             0.48,
                             {
                                 make_tuple<Stock, uint32_t>({"stock_1", 54., 0.25f, 658.}, 200),
                                 make_tuple<Stock, uint32_t>({"stock_2", 86., 0.45f, 477.}, 100),
                             }};

            it("Check csv serializer", [&](){
                stringstream result;
                CsvSerializer{}(result, output);
                AssertThat(result.str(), Equals("546510\n78415\n0.48\nstock_1;200\nstock_2;100\n"));
            });
        });

        describe("Json deserializer test", [&](){
            stringstream json_stream(R"(
            {
                "budget": 50000,
                "stocks": [{"name": "stock_1", "price": 500, "probability": 0.47, "incoming": 10000, "count": 102},
                           {"name": "stock_2", "price": 1500, "probability": 0.88, "incoming": 1800, "count": 150}]
            })");

            it("Check json deserializer", [&](){
                Input inputData = JsonDeserializer<Input>{}(json_stream);
                AssertThat(inputData.budget, EqualsWithDelta(50000.,
                                                             numeric_limits<double>::epsilon()));
                AssertThat(inputData.stocks.size(), Equals(2));


                AssertThat(get<0>(inputData.stocks[0]).name, Equals("stock_1"));
                AssertThat(get<0>(inputData.stocks[0]).price, EqualsWithDelta(500.,
                                                                              numeric_limits<double>::epsilon()));
                AssertThat(get<0>(inputData.stocks[0]).probability, EqualsWithDelta(0.47,
                                                                                    0.001));
                AssertThat(get<0>(inputData.stocks[0]).incoming, EqualsWithDelta(10000.,
                                                                                 numeric_limits<double>::epsilon()));
                AssertThat(get<1>(inputData.stocks[0]), Equals(102));


                AssertThat(get<0>(inputData.stocks[1]).name, Equals("stock_2"));
                AssertThat(get<0>(inputData.stocks[1]).price, EqualsWithDelta(1500.,
                                                                              numeric_limits<double>::epsilon()));
                AssertThat(get<0>(inputData.stocks[1]).probability, EqualsWithDelta(0.88,
                                                                                    0.001));
                AssertThat(get<0>(inputData.stocks[1]).incoming, EqualsWithDelta(1800.,
                                                                                 numeric_limits<double>::epsilon()));
                AssertThat(get<1>(inputData.stocks[1]), Equals(150));
            });
        });

        describe("Csv deserializer test", [&](){
            stringstream json_stream("50000\nstock_1;500;0.47;10000;102\nstock_2;1500;0.88;1800;150");

            it("Check csv deserializer", [&](){
                Input inputData = CsvDeserializer<Input>{}(json_stream);
                AssertThat(inputData.budget, EqualsWithDelta(50000.,
                                                             numeric_limits<double>::epsilon()));
                AssertThat(inputData.stocks.size(), Equals(2));


                AssertThat(get<0>(inputData.stocks[0]).name, Equals("stock_1"));
                AssertThat(get<0>(inputData.stocks[0]).price, EqualsWithDelta(500.,
                                                                              numeric_limits<double>::epsilon()));
                AssertThat(get<0>(inputData.stocks[0]).probability, EqualsWithDelta(0.47,
                                                                                    0.001));
                AssertThat(get<0>(inputData.stocks[0]).incoming, EqualsWithDelta(10000.,
                                                                                 numeric_limits<double>::epsilon()));
                AssertThat(get<1>(inputData.stocks[0]), Equals(102));


                AssertThat(get<0>(inputData.stocks[1]).name, Equals("stock_2"));
                AssertThat(get<0>(inputData.stocks[1]).price, EqualsWithDelta(1500.,
                                                                              numeric_limits<double>::epsilon()));
                AssertThat(get<0>(inputData.stocks[1]).probability, EqualsWithDelta(0.88,
                                                                                    0.001));
                AssertThat(get<0>(inputData.stocks[1]).incoming, EqualsWithDelta(1800.,
                                                                                 numeric_limits<double>::epsilon()));
                AssertThat(get<1>(inputData.stocks[1]), Equals(150));
            });
        });
    });
}