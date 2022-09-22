#include "Parser.h"

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

bool Parser::CheckFileName(const std::string& input, const std::string& output) const {
    bool flag_input = input.substr(input.size() - STRING_BMP_SIZE, STRING_BMP_SIZE) != ".bmp";
    bool flag_output = output.substr(output.size() - STRING_BMP_SIZE, STRING_BMP_SIZE) != ".bmp";
    return flag_input || flag_output;
}

void Parser::CheckIsRightInputOutputFile(const std::string& input, const std::string& output) const {
    if (CheckFileName(input, output)) {
        throw std::invalid_argument("Input and Output files should be FILENAME.bmp");
    }
}

int Parser::CheckIsEnoughFilterArgument(const std::string& filter_name, FilterID filter_id, int count_argument) const {
    int count_filter_argument = 0;
    std::string invalid_message;
    switch (filter_id) {
        case FilterID::CROP: {
            count_filter_argument = CountFilterArguments::CROP_COUNT;
            invalid_message = "Crop has 2 arguments";
            break;
        }
        case FilterID::GRAYSCALE: {
            count_filter_argument = CountFilterArguments::GRAYSCALE_COUNT;
            break;
        }
        case FilterID::NEGATIVE: {
            count_filter_argument = CountFilterArguments::NEGATIVE_COUNT;
            break;
        }
        case FilterID::SHARPENING: {
            count_filter_argument = CountFilterArguments::SHARPENING_COUNT;
            break;
        }
        case FilterID::EDGE_DETECTION: {
            count_filter_argument = CountFilterArguments::EDGE_DETECTION_COUNT;
            invalid_message = "Edge has 1 argument";
            break;
        }
    }

    if (count_argument >= count_filter_argument) {
        return count_filter_argument;
    }

    throw std::invalid_argument("Not enough " + filter_name + " arguments");
}

void Parser::ParsingArguments(int argc, const char**& argv, ProgramArguments& arguments) const {
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

