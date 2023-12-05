#include "BMP.h"

#include <fstream>

Colour::Colour() {
    r = 0;
    g = 0;
    b = 0;
}

Colour::Colour(float r_construct, float g_construct, float b_construct) {
    r = r_construct;
    b = b_construct;
    g = g_construct;
}

Colour Image::GetColour(uint16_t x, uint16_t y) const {
    return colours[y * length + x];
}

void Image::SetColour(uint16_t x, uint16_t y, const Colour& colour) {
    colours[y * length + x].r = colour.r;
    colours[y * length + x].g = colour.g;
    colours[y * length + x].b = colour.b;
}

Image::Image(Sandpile* sandpile) {
    length = sandpile -> length;
    width = sandpile -> width;
    colours = new Colour[width * length];
    uint64_t grains = 0;
    for (int y = 0; y < width; y++) {
        for (int x = 0; x < length; x++) {
            if (sandpile -> barriers.find(std::make_pair(x, y)) != sandpile -> barriers.end()) {
                SetColour(x, y, Colour(255, 255, 255));
            }
            else {
                grains = sandpile->pile[y][x];
                if (grains == 0) {
                    SetColour(x, y, Colour(255, 255, 255));
                } else if (grains == 1) {
                    SetColour(x, y, Colour(0, 255, 0));
                } else if (grains == 2) {
                    SetColour(x, y, Colour(139, 0, 255));
                } else if (grains == 3) {
                    SetColour(x, y, Colour(255, 255, 0));
                } else {
                    SetColour(x, y, Colour());
                }
            }
        }
    }
}

void Image::BuildImage(const std::string& path, const std::string name) {
    std::ofstream output_file;
    std::string this_path = path + "\\" + name + ".bmp";
    output_file.open(this_path, std::ios::out | std::ios::binary);
    if (!output_file.is_open()) {
        std::cout << "Couldn't open output file" << std::endl;
        return;
    }
    const int kOneElementSize = 3;
    const int kNeededLengthModule = 4;
    unsigned char padding[kOneElementSize] = {0, 0, 0};
    const int kPaddingAmount = (kNeededLengthModule - (kOneElementSize * width) % kNeededLengthModule) % kNeededLengthModule;

    const int kFileHeaderSize = 14;
    const int kInformationHeaderSize = 40;
    const int kFileSize = kFileHeaderSize + kInformationHeaderSize + width * (length * kOneElementSize + kPaddingAmount);

    unsigned char fileHeader[kFileHeaderSize] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0,
                                                 0, 0, 0, 0};


    fileHeader[2] = kFileSize;
    fileHeader[3] = kFileSize >> 8;
    fileHeader[4] = kFileSize >> 16;
    fileHeader[5] = kFileSize >> 24;
    fileHeader[10] = kFileHeaderSize +kInformationHeaderSize;

    unsigned char informationHeader[kInformationHeaderSize] = {0, 0, 0, 0, 0, 0, 0, 0,
                                                               0, 0, 0, 0, 1, 0, 24};
    for (int i = 15; i < 40; i++) {
        informationHeader[i] = 0;
    }

    output_file.write(reinterpret_cast<char*>(fileHeader), kFileHeaderSize);
    output_file.write(reinterpret_cast<char*>(informationHeader), kInformationHeaderSize);

    for (int y = width - 1; y >= 0; y--) {
        for (int x = 0; x < length; x++) {
            Colour current_colour = GetColour(x, y);

            unsigned char r = static_cast<unsigned char>(current_colour.r);
            unsigned char g = static_cast<unsigned char>(current_colour.g);
            unsigned char b = static_cast<unsigned char>(current_colour.b);
            unsigned char colour[] = {b, g, r};

            output_file.write(reinterpret_cast<char*>(colour), 3);
        }
        output_file.write(reinterpret_cast<char*>(padding), kPaddingAmount);
    }
}
