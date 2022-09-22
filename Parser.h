#pragma once

#include "Filter.h"

constexpr uint COUNT_REQUIRED_PARAMETERS = 3;
constexpr uint INDEX_INPUT_FILE = 1;
constexpr uint INDEX_OUTPUT_FILE = 2;
constexpr uint STRING_BMP_SIZE = 4;

struct ProgramArguments {
    std::string input_file;
    std::string output_file;
    std::vector<Filter> filters;
};

FilterID GetFilterID(const std::string& filter_name) {
    if (filter_name == "-crop") {
        return FilterID::CROP;
    }

    if (filter_name == "-gs") {
        return FilterID::GRAYSCALE;
    }

    if (filter_name == "-neg") {
        return FilterID::NEGATIVE;
    }

    if (filter_name == "-sharp") {
        return FilterID::SHARPENING;
    }

    if (filter_name == "-edge") {
        return FilterID::EDGE_DETECTION;
    }
    throw std::invalid_argument("Unknown filter name " + filter_name);
}

class Parser {
public:
    Parser() = default;

    void ParsingArguments(int argc, const char**& argv, ProgramArguments& arguments) const {
        if (argc < COUNT_REQUIRED_PARAMETERS) {
            throw std::invalid_argument("Not enough arguments");
        }

        arguments.input_file = argv[INDEX_INPUT_FILE];
        arguments.output_file = argv[INDEX_OUTPUT_FILE];
        CheckIsRightInputOutputFile(arguments.input_file, arguments.output_file);

        int i = COUNT_REQUIRED_PARAMETERS;
        while (i < argc) {
            std::string filter_name = argv[i];
            FilterID filter_id = GetFilterID(filter_name);
            int count_filter_arguments = CheckIsEnoughFilterArgument(filter_name, filter_id, argc - i);
            Filter filter{.filter_name = filter_name};

            switch (filter_id) {
                case FilterID::CROP: {
                    filter.arguments.emplace_back(argv[i + 1]);
                    filter.arguments.emplace_back(argv[i + 2]);
                    break;
                }
                case FilterID::GRAYSCALE:
                case FilterID::NEGATIVE:
                case FilterID::SHARPENING: {
                    break;
                }
                case FilterID::EDGE_DETECTION: {
                    filter.arguments.emplace_back(argv[i + 1]);
                    break;
                }
            }

            arguments.filters.push_back(filter);
            i += count_filter_arguments;
        }
    }

private:
    [[nodiscard]] bool CheckFileName(const std::string& input, const std::string& output) const;

    void CheckIsRightInputOutputFile(const std::string& input, const std::string& output) const;

    [[nodiscard]] int
    CheckIsEnoughFilterArgument(const std::string& filter_name, FilterID filter_id, int count_argument) const;

};