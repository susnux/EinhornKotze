#include <cstdint>
#include "font8x8_basic.h"
#include "font8x8_ext_latin.h"

#include <fstream>
#include <iostream>

void convert_row_column(std::ofstream& output, const uint8_t* input, uint8_t width, uint8_t height)
{
    for (uint8_t column = 0; column < width; ++column) {
        uint8_t c = 0;
        for (auto row = 0; row < height; ++row) {
            c |= (((input[row] >> column) & 1) << row);
        }
        output.put(c);
    }
}

int main(int argc, char** argv) {
    std::string filename = "font8x8.bin";

    if (argc > 1) filename = std::string(argv[1]);
    std::ofstream output(filename, std::ios::binary | std::ios::out);
    uint8_t width = 8;
    uint8_t height = 8;
    output.put(width);
    output.put(height);

    for (auto i = 0; i < 128; ++i) {
        convert_row_column(output, font8x8_basic[i], width, height);
    }
    for (auto i = 128; i < 0xA0; ++i) {
        char c = '\0';
        for (auto j = 0; j < width; ++j) output.put(c);
    }
    for (auto i = 0; i < 96; ++i) {
        convert_row_column(output, font8x8_ext_latin[i], width, height);        
    }
    output.close();
}