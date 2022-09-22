#include "Image.h"
#include <stdexcept>

uint HexToInt(const char* x, uint start, uint finish) {
    uint tmp = 0;
    for (size_t i = finish - 1; i >= start; --i) {
        tmp <<= BITS_PER_BYTE;
        tmp += x[i];
    }
    return tmp;
}

void WriteByte(std::ostream& out, uint8_t value) {
    out.write(reinterpret_cast<char*>(&value), 1);
}

void WriteInt(std::ostream& out, uint32_t value) {
    for (uint i = 0; i < BYTES_PER_INT; ++i) {
        WriteByte(out, (value >> (i * BITS_PER_BYTE)) & BYTE_MASK);
    }
}

void WriteZeros(std::ostream& out, uint count) {
    for (uint i = 0; i < count; ++i) {
        WriteByte(out, 0);
    }
}


void WriteColor(std::ostream& out, const Color& color) {
    WriteByte(out, color.blue);
    WriteByte(out, color.green);
    WriteByte(out, color.red);
}

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return height_;
}

Color Image::GetColor(size_t i, size_t j) const {
    if (i < height_ && j < width_) {
        return pixels_[i][j];
    }
    return {};
}

void Image::SetColor(size_t i, size_t j, Color newPixel) {
    pixels_[i][j] = newPixel;
}

void Image::ReadImage(const std::string& input_file_name) {
    std::ifstream input(input_file_name, std::ios::binary);
    char header[BmpHeader::HEADER_SIZE];
    input.read(header, BmpHeader::HEADER_SIZE);

    if (!input) {
        throw std::invalid_argument("Can't read image");
    }

    if (header[0] != BmpHeader::BMP_SIGNATURE_BYTE_1 || header[1] != BmpHeader::BMP_SIGNATURE_BYTE_2) {
        throw std::invalid_argument("Can't find bmp magic header");
    }

    width_ = HexToInt(header, WIDTH_INDEX_START, WIDTH_INDEX_END);
    height_ = HexToInt(header, HEIGHT_INDEX_START, HEIGHT_INDEX_END);

    uint data_offset = HexToInt(header, DATA_OFFSET_INDEX_START, DATA_OFFSET_INDEX_END);
    uint bit_count = HexToInt(header, BIT_COUNT_INDEX_START, BIT_COUNT_INDEX_END);

    ReadPixels(input, bit_count, data_offset);
}

void Image::ReadPixels(std::ifstream& input, uint bit_count, uint data_offset) {
    uint byte_count = bit_count / BITS_PER_BYTE;

    Pixels colors(height_, std::vector<Color>(width_));

    uint real_size = byte_count * width_;
    uint padding = (BYTES_PER_INT - real_size % BYTES_PER_INT) % BYTES_PER_INT;
    uint start = data_offset;
    uint finish = start + real_size;
    for (size_t i = 0; i < height_; ++i) {
        input.seekg(start);
        for (size_t j = start; j < finish;) {
            input.read(reinterpret_cast<char*>(&colors[i][(j - start) / COUNT_COLORS].blue), COUNT_BYTE);
            input.read(reinterpret_cast<char*>(&colors[i][(j - start) / COUNT_COLORS].green), COUNT_BYTE);
            input.read(reinterpret_cast<char*>(&colors[i][(j - start) / COUNT_COLORS].red), COUNT_BYTE);
            j += COUNT_COLORS;
        }
        start = finish + padding;
        finish = start + real_size;
    }
    pixels_ = colors;
    std::reverse(pixels_.begin(), pixels_.end());
    input.close();
}

void Image::WriteBMPHEADER(std::ostream& out) const {
    WriteByte(out, BmpHeader::BMP_SIGNATURE_BYTE_1);
    WriteByte(out, BmpHeader::BMP_SIGNATURE_BYTE_2);

    uint padding = (BYTES_PER_INT - width_ * COUNT_COLORS % BYTES_PER_INT) % BYTES_PER_INT;

    uint file_size = BmpHeader::HEADER_SIZE + height_ * (width_ * COUNT_COLORS + padding);
    WriteInt(out, file_size);
    WriteZeros(out, COUNT_DATA_OFFSET);
    WriteInt(out, BmpHeader::HEADER_SIZE);
    WriteInt(out, BmpHeader::INFO_HEADER_SIZE);
    WriteInt(out, width_);
    WriteInt(out, height_);
    // planes = 01; 1 0
    WriteByte(out, 1);
    WriteZeros(out, 1);
    // bit_per_pixel = [0, 24]
    WriteByte(out, BmpHeader::BITS_PER_PIXEL);
    WriteZeros(out, 1);
    WriteZeros(out, BmpHeader::AFTER_IMAGE_PARAMS_BYTE_COUNT);
}

void Image::WriteImage(const std::string& output_file_name) {
    std::ofstream out(output_file_name, std::ios::binary);
    std::reverse(pixels_.begin(), pixels_.end());
    WriteBMPHEADER(out);

    for (const auto& row : pixels_) {
        for (const auto& color : row) {
            WriteColor(out, color);
        }
    }
    out.close();
}