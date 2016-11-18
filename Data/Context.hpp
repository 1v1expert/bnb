#ifndef BNB_CONTEXT_HPP
#define BNB_CONTEXT_HPP

#include <istream>
#include <ostream>

namespace bnb
{
    using std::istream;
    using std::ostream;

    template<typename Input_t,
             typename Output_t>
    struct DataContext
    {
        Input_t input_data;
        Output_t output_data;

        template<typename Deserializer_t>
        Input_t& InputDataLoad(istream& stream)
        {
            input_data = Deserializer_t{}.operator()(stream);
            return input_data;
        }

        template<typename Mutator_t>
        void Mutate(Mutator_t& mutator)
        {
            output_data = mutator(input_data);
        }

        template<typename Serializer_t>
        void OutputDataSave(ostream& stream)
        {
            Serializer_t{}(stream, output_data);
        }
    };
}

#endif //BNB_CONTEXT_HPP
