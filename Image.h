#pragma once

#include <fstream>
#include <vector>

namespace BmpHeader {
    constexpr uint BMP_SIGNATURE_BYTE_1 = 0x42;
    constexpr uint BMP_SIGNATURE_BYTE_2 = 0x4D;
    constexpr uint HEADER_SIZE = 54;
    constexpr uint INFO_HEADER_SIZE = 40;
    constexpr uint BITS_PER_PIXEL = 24;
    constexpr uint AFTER_IMAGE_PARAMS_BYTE_COUNT = 24;
}


constexpr uint BITS_PER_BYTE = 8;
constexpr uint BYTES_PER_INT = 4;
constexpr uint BYTE_MASK = 0xFF;

constexpr uint COUNT_COLORS = 3;
constexpr uint COUNT_BYTE = 1;
constexpr uint COUNT_DATA_OFFSET = 4;

constexpr uint WIDTH_INDEX_START = 18;
constexpr uint WIDTH_INDEX_END = 22;

constexpr uint HEIGHT_INDEX_START = 22;
constexpr uint HEIGHT_INDEX_END = 26;

constexpr uint DATA_OFFSET_INDEX_START = 10;
constexpr uint DATA_OFFSET_INDEX_END = 14;

constexpr uint BIT_COUNT_INDEX_START = 28;
constexpr uint BIT_COUNT_INDEX_END = 30;

constexpr uint8_t MAX_COLOR_VALUE = 255;
constexpr uint8_t MIN_COLOR_VALUE = 0;

struct Color {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};

using Pixels = std::vector<std::vector<Color>>;

class Image {
public:
    Image() = default;

    [[nodiscard]] size_t GetWidth() const;

    [[nodiscard]] size_t GetHeight() const;

    [[nodiscard]] Color GetColor(size_t i, size_t j) const;

    void SetColor(size_t i, size_t j, Color newPixel);

    void ReadImage(const std::string& input_file_name);

    void ReadPixels(std::ifstream& input, uint bit_count, uint data_offset);

    void WriteBMPHEADER(std::ostream& out) const;

    void WriteImage(const std::string& output_file_name);

    ~Image() = default;

    Pixels pixels_;
    size_t width_ = 0;
    size_t height_ = 0;
};