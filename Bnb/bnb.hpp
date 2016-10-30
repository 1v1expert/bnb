#ifndef BNB_BNB_HPP
#define BNB_BNB_HPP

#include "../Data/Input.hpp"
#include "../Data/Output.hpp"

namespace bnb
{
    struct Bnb
    {
        Output operator()(Input& input)
        {
            return {};
        }
    };
}

#endif //BNB_BNB_HPP
