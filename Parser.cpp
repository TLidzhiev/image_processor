#include "Parser.h"

bool Parser::CheckFileName(const std::string& input, const std::string& output) const {
    bool flag_input = input.substr(input.size() - STRING_BMP_SIZE, STRING_BMP_SIZE) != ".bmp";
    bool flag_output = output.substr(output.size() - STRING_BMP_SIZE, STRING_BMP_SIZE) != ".bmp";
    return flag_input || flag_output;
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

void Parser::CheckIsRightInputOutputFile(const std::string& input, const std::string& output) const {
    if (CheckFileName(input, output)) {
        throw std::invalid_argument("Input and Output files should be FILENAME.bmp");
    }
}
