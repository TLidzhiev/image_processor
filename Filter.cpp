#include "Filter.h"

Crop::Crop(const std::vector<std::string>& filter_arguments) {
    if (filter_arguments.size() != CountFilterArguments::CROP_COUNT - 1) {// without name
        throw std::invalid_argument("Crop has 2 arguments");
    }

    int64_t temp_w = std::stoi(filter_arguments[0]);
    int64_t temp_h = std::stoi(filter_arguments[1]);
    if (temp_w < 0 || temp_h < 0) {
        throw std::invalid_argument("Crop has 2 positive integer arguments");
    }

    width_ = static_cast<size_t>(temp_w);
    height_ = static_cast<size_t>(temp_h);
}

void Crop::ApplyFilter(Image& image) const {
    if (width_ > image.width_ || height_ > image.height_) {
        throw std::invalid_argument("Crop arguments > image size");
    }
    image.pixels_.resize(height_);
    for (auto& row : image.pixels_) {
        row.resize(width_);
    }
    image.height_ = image.pixels_.size();
    image.width_ = image.pixels_[0].size();
}


void Grayscale::ApplyFilter(Image& image) const {
    for (size_t i = 0; i < image.height_; ++i) {
        for (size_t j = 0; j < image.width_; ++j) {
            Color current_pixel = image.pixels_[i][j];
            auto newColor = static_cast<uint8_t>(current_pixel.red * red +
                                                 current_pixel.green * green +
                                                 current_pixel.blue * blue
            );

            current_pixel.red = newColor;
            current_pixel.green = newColor;
            current_pixel.blue = newColor;

            image.pixels_[i][j] = current_pixel;
        }
    }
}

void Negative::ApplyFilter(Image& image) const {
    size_t height = image.height_;
    size_t width = image.width_;

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            Color current_pixel = image.pixels_[i][j];
            current_pixel.red = MAX_COLOR_VALUE - current_pixel.red;
            current_pixel.green = MAX_COLOR_VALUE - current_pixel.green;
            current_pixel.blue = MAX_COLOR_VALUE - current_pixel.blue;

            image.pixels_[i][j] = current_pixel;
        }
    }
}

template<typename Type>
void GetNewPixel(const Image& image, Pixels& new_pixels, const Matrix<Type>& matrix_filter, size_t row, size_t column) {
    size_t height_center = matrix_filter.size() / 2;
    size_t width_center = matrix_filter[0].size() / 2;
    Type red = 0;
    Type green = 0;
    Type blue = 0;
    for (size_t k = 0; k < matrix_filter.size(); ++k) {
        for (size_t l = 0; l < matrix_filter[0].size(); ++l) {
            int y = std::clamp(static_cast<int>(column + l - height_center), 0, static_cast<int>(image.width_ - 1));
            int x = std::clamp(static_cast<int>(row + k - width_center), 0, static_cast<int>(image.height_ - 1));
            red += matrix_filter[k][l] * image.pixels_[x][y].red;
            green += matrix_filter[k][l] * image.pixels_[x][y].green;
            blue += matrix_filter[k][l] * image.pixels_[x][y].blue;
        }
    }
    new_pixels[row][column].red = std::clamp(static_cast<int>(red),
                                             static_cast<int>(MIN_COLOR_VALUE),
                                             static_cast<int>(MAX_COLOR_VALUE)
    );

    new_pixels[row][column].green = std::clamp(static_cast<int>(green),
                                               static_cast<int>(MIN_COLOR_VALUE),
                                               static_cast<int>(MAX_COLOR_VALUE)
    );

    new_pixels[row][column].blue = std::clamp(static_cast<int>(blue),
                                              static_cast<int>(MIN_COLOR_VALUE),
                                              static_cast<int>(MAX_COLOR_VALUE)
    );

}


template<typename Type>
void ApplyMatrix(Image& image, const Matrix<Type>& matrix_filter) {
    Pixels new_pixels(image.height_, std::vector<Color>(image.width_));
    for (size_t i = 0; i < image.height_; ++i) {
        for (size_t j = 0; j < image.width_; ++j) {
            GetNewPixel(image, new_pixels, matrix_filter, i, j);
        }
    }
    std::move(new_pixels.begin(), new_pixels.end(), image.pixels_.begin());
}


void Sharpening::ApplyFilter(Image& image) const {
    ApplyMatrix(image, matrix_);
}


EdgeDetection::EdgeDetection(const std::vector<std::string>& filter_arguments) {
    if (filter_arguments.size() != CountFilterArguments::EDGE_DETECTION_COUNT - 1) {// without name
        throw std::invalid_argument("Edge has 1 arguments");
    }
    threshold_ = std::stoi(filter_arguments[0]);
}


void EdgeDetection::ApplyFilter(Image& image) const {
    Grayscale grayscale;
    grayscale.ApplyFilter(image);

    ApplyMatrix(image, matrix_);
    for (size_t i = 0; i < image.height_; ++i) {
        for (size_t j = 0; j < image.width_; ++j) {
            if (image.pixels_[i][j].red > threshold_) {
                image.pixels_[i][j].red = MAX_COLOR_VALUE;
                image.pixels_[i][j].green = MAX_COLOR_VALUE;
                image.pixels_[i][j].blue = MAX_COLOR_VALUE;

            } else if (image.pixels_[i][j].red < threshold_) {
                image.pixels_[i][j].red = MIN_COLOR_VALUE;
                image.pixels_[i][j].green = MIN_COLOR_VALUE;
                image.pixels_[i][j].blue = MIN_COLOR_VALUE;
            }
        }
    }
}