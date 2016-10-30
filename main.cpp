#include <iostream>
#include <map>
#include <fstream>
#include <memory>
#include <cstring>
#include <bandit/bandit.h>

#include "Bnb/bnb.hpp"
#include "Data/Context.hpp"
#include "Data/Input.hpp"
#include "Data/Output.hpp"
#include "Data/Deserializer.hpp"
#include "Data/Serializer.hpp"

using std::string;
using std::map;
using std::string;
using std::ostream;
using std::fstream;
using std::ios;

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::move;
using std::make_unique;

enum class WorkingMode
{
    Help, InputPath, OutputPath, Test
};

struct ParametersProcessor
{
    static bool ProcessInput(int argc, char ** argv)
    {
        if (argc == 1) return false;

        for (auto i = 0; i < argc; ++i)
        {
            if (strcmp(argv[i], "--help") == 0)
            {
                input_map[WorkingMode::Help] = "";
            }
            else if (strcmp(argv[i], "--input") == 0 && i + 1 < argc)
            {
                input_map[WorkingMode::InputPath] = { argv[i + 1] };
            }
            else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc)
            {
                input_map[WorkingMode::OutputPath] = { argv[i + 1] };
            }
            else if (strcmp(argv[i], "--test") == 0)
            {
                input_map[WorkingMode::Test] = "";
            }
        }

        return true;
    }

    static ostream& Help(ostream& stream)
    {
        stream << "Available options:\n"
               << "--help (Print available options)\n"
               << "--input [path] (Input path for csv file)\n"
               << "--output [path] (Output path (task solution))\n"
               << "--test (Run unit tests)\n";
        return stream;
    }

    static map<WorkingMode, string> input_map;
};

map<WorkingMode, string> ParametersProcessor::input_map;

int main(int argc, char ** argv)
{
    if (!ParametersProcessor::ProcessInput(argc, argv))
    {
        cerr << "No input parameters. Type --help to see available input parameters"
             << endl;
        return -1;
    }
    if (ParametersProcessor::input_map.count(WorkingMode::Help))
    {
        cout << ParametersProcessor::Help;
        return 0;
    }
    if (ParametersProcessor::input_map.count(WorkingMode::Test))
    {
        return bandit::run(argc, argv);
    }
    if (ParametersProcessor::input_map.count(WorkingMode::InputPath)
        && ParametersProcessor::input_map.count(WorkingMode::OutputPath))
    {
        fstream input_file(ParametersProcessor::input_map[WorkingMode::InputPath],
                           ios::in);
        fstream output_file(ParametersProcessor::input_map[WorkingMode::OutputPath],
                            ios::out | ios::trunc);

        if (input_file.is_open() && output_file.is_open())
        {
            if (input_file.peek() == fstream::traits_type::eof())
            {
                cerr << "Input file is empty:\n"
                     << ParametersProcessor::input_map[WorkingMode::InputPath]
                     << endl;
                return -1;
            }

            auto data_context = make_unique<bnb::DataContext<bnb::Input, bnb::Output>>();
            data_context->InputDataLoad<bnb::JsonDeserializer>(input_file);
            bnb::Bnb b;
            data_context->Mutate(b);
            data_context->OutputDataSave<bnb::JsonSerializer>(cout);
            data_context->OutputDataSave<bnb::JsonSerializer>(output_file);
        }
        else
        {
            cerr << "Unable to open file in one of given paths:\n"
                 << ParametersProcessor::input_map[WorkingMode::InputPath]
                 << endl
                 << ParametersProcessor::input_map[WorkingMode::OutputPath]
                 << endl;
            return -1;
        }
    }
    else
    {
        cerr << "No such parameters. Maybe you forget input or output path?\n"
                "Type --help to see available input parameters"
             << endl;
        return -1;
    }

    return 0;
}