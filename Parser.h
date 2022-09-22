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

FilterID GetFilterID(const std::string& filter_name);

class Parser {
public:
    Parser() = default;

    void ParsingArguments(int argc, const char**& argv, ProgramArguments& arguments) const;

private:
    bool CheckFileName(const std::string& input, const std::string& output) const;

    void CheckIsRightInputOutputFile(const std::string& input, const std::string& output) const;

    int CheckIsEnoughFilterArgument(const std::string& filter_name, FilterID filter_id, int count_argument) const;

};