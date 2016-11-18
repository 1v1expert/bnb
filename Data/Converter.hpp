#ifndef BNB_CONVERTER_HPP
#define BNB_CONVERTER_HPP

#include <string>

using std::string;

namespace bnb
{
    template<typename DistanceType>
    struct Converter
    {
        static DistanceType Convert(string value) { return {}; }
    };

    template<>
    struct Converter<int32_t>
    {
        static int32_t Convert(string value)
        {
            try
            {
                return stoi(value);
            }
            catch (const std::out_of_range&)
            {
                return 0;
            }
            catch (const std::invalid_argument&)
            {
                return 0;
            }
        }
    };

    template<>
    struct Converter<float>
    {
        static float Convert(string value)
        {
            try
            {
                return stof(value);
            }
            catch (const std::out_of_range&)
            {
                return 0.f;
            }
            catch (const std::invalid_argument&)
            {
                return 0.f;
            }
        }
    };

    template<>
    struct Converter<double>
    {
        static double Convert(string value)
        {
            try
            {
                return stod(value);
            }
            catch (const std::out_of_range&)
            {
                return 0.f;
            }
            catch (const std::invalid_argument&)
            {
                return 0.f;
            }
        }
    };
}

#endif //BNB_CONVERTER_HPP
