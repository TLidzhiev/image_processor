#pragma once

#include "Image.h"

template<typename Type>
using Matrix = std::vector<std::vector<Type>>;

struct Filter {
    std::string filter_name;
    std::vector<std::string> arguments;
};

enum FilterID {
    CROP,
    GRAYSCALE,
    NEGATIVE,
    SHARPENING,
    EDGE_DETECTION
};

enum CountFilterArguments {
    CROP_COUNT = 3,          // filter_name + wight + height
    GRAYSCALE_COUNT = 1,     // filter_name
    NEGATIVE_COUNT = 1,      // filter_name
    SHARPENING_COUNT = 1,    // filter_name
    EDGE_DETECTION_COUNT = 2,// filter_name + threshold
};


class BaseFilter {
public:
    BaseFilter() = default;

    virtual void ApplyFilter(Image& image) const = 0;

    ~BaseFilter() = default;
};

class Crop : public BaseFilter {
public:
    explicit Crop(const std::vector<std::string>& filter_arguments);

    void ApplyFilter(Image& image) const override;

private:
    size_t width_ = 0;
    size_t height_ = 0;
};

class Grayscale : public BaseFilter {
public:
    Grayscale() = default;

    void ApplyFilter(Image& image) const override;

private:
    const long double red = 0.299;
    const long double green = 0.587;
    const long double blue = 0.114;
};

class Negative : public BaseFilter {
public:
    Negative() = default;

    void ApplyFilter(Image& image) const override;
};

class Sharpening : public BaseFilter {
public:
    Sharpening() = default;

    void ApplyFilter(Image& image) const override;

private:
    const Matrix<int> matrix_ = {{0,  -1, 0},
                                 {-1, 5,  -1},
                                 {0,  -1, 0}};
};

class EdgeDetection : public BaseFilter {
public:
    EdgeDetection() = default;

    explicit EdgeDetection(const std::vector<std::string>& filter_arguments);

    void ApplyFilter(Image& image) const override;

private:
    const Matrix<int> matrix_ = {{0,  -1, 0},
                                 {-1, 4,  -1},
                                 {0,  -1, 0}};
    uint8_t threshold_ = 0;
};