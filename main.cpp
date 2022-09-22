#include <iostream>
#include <memory>
#include "Parser.h"

void FilterImplementation(const std::vector<Filter>& filters, std::vector<std::shared_ptr<BaseFilter>>& real_filter) {
    for (const auto& filter : filters) {
        FilterID filter_id = GetFilterID(filter.filter_name);
        switch (filter_id) {
            case FilterID::CROP: {
                real_filter.push_back(std::make_shared<Crop>(filter.arguments));
                break;
            }
            case FilterID::GRAYSCALE: {
                real_filter.push_back(std::make_shared<Grayscale>());
                break;
            }
            case FilterID::NEGATIVE: {
                real_filter.push_back(std::make_shared<Negative>());
                break;
            }
            case FilterID::SHARPENING: {
                real_filter.push_back(std::make_shared<Sharpening>());
                break;
            }
            case FilterID::EDGE_DETECTION: {
                real_filter.push_back(std::make_shared<EdgeDetection>(filter.arguments));
                break;
            }
        }
    }
}

void Apply(const std::vector<std::shared_ptr<BaseFilter>>& filters, Image& image) {
    for (const auto& filter : filters) {
        filter->ApplyFilter(image);
    }
}

int main(int argc, const char** argv) {
    ProgramArguments args;
    Parser parser;
    try {
        parser.ParsingArguments(argc, argv, args);
    } catch (const std::exception& exception) {
        std::cout << "Parsing error: " << exception.what() << std::endl;
        return 1;
    }

    Image image;
    try {
        image.ReadImage(args.input_file);
    } catch (const std::exception& exception) {
        std::cout << "Read image error: " << exception.what() << std::endl;
        return 1;
    }

    std::vector<std::shared_ptr<BaseFilter>> real_filters;
    try {
        FilterImplementation(args.filters, real_filters);
    } catch (const std::exception& exception) {
        std::cout << "Filter implementation error: " << exception.what() << std::endl;
        return 1;
    }

    try {
        Apply(real_filters, image);
    } catch (const std::exception& exception) {
        std::cout << "Filter apply error: " << exception.what() << std::endl;
        return 1;
    }
    image.WriteImage(args.output_file);
    return 0;
}