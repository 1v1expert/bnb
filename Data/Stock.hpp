#ifndef BNB_STOCK_HPP
#define BNB_STOCK_HPP

#include <string>

namespace bnb
{
    using std::string;

    struct Stock
    {
        string name;
        double price;
        float probability;
        double incoming;

        friend bool operator==(const Stock& lhs,
                               const Stock& rhs)
        {
            return lhs.name == rhs.name
                   && lhs.price == rhs.price
                   && lhs.probability == rhs.probability
                   && lhs.incoming == rhs.incoming;
        }
    };
}

#endif //BNB_STOCK_HPP
