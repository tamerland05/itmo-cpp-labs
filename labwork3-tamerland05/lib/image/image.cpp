#include "image.h"

Image::Image() {
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';

    fileHeader[10] = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE;

    informationHeader[0] = INFORMATION_HEADER_SIZE;

    informationHeader[12] = 1;

    informationHeader[14] = SIZE_OF_COLOR; // бит на пиксель

    informationHeader[32] = 16; // цветов в палитре

    Palette[0][0] = 255;

    Palette[0][1] = 255;
    Palette[0][2] = 255;

    Palette[1][1] = 128;

    Palette[2][1] = 255;
    Palette[2][2] = 255;

    Palette[3][0] = 255;
    Palette[3][2] = 139;
}

void Image::UpdateSize() {
    const uint64_t fileSize = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + width * height;

    fileHeader[2] = static_cast<unsigned char>(fileSize % MAX_VALUE_OF_BYTE);
    fileHeader[3] = static_cast<unsigned char>(fileSize >> SHIFT_IN_SECOND_BYTE % MAX_VALUE_OF_BYTE);
    fileHeader[4] = static_cast<unsigned char>(fileSize >> SHIFT_IN_THIRD_BYTE % MAX_VALUE_OF_BYTE);
    fileHeader[5] = static_cast<unsigned char>(fileSize >> SHIFT_IN_FOURTH_BYTE % MAX_VALUE_OF_BYTE);

    informationHeader[4] = static_cast<unsigned char>(width % MAX_VALUE_OF_BYTE);
    informationHeader[5] = static_cast<unsigned char>(width >> SHIFT_IN_SECOND_BYTE % MAX_VALUE_OF_BYTE);
    informationHeader[6] = static_cast<unsigned char>(width >> SHIFT_IN_THIRD_BYTE % MAX_VALUE_OF_BYTE);
    informationHeader[7] = static_cast<unsigned char>(width >> SHIFT_IN_FOURTH_BYTE % MAX_VALUE_OF_BYTE);

    informationHeader[8] = static_cast<unsigned char>(height % MAX_VALUE_OF_BYTE);
    informationHeader[9] = static_cast<unsigned char>(height >> SHIFT_IN_SECOND_BYTE % MAX_VALUE_OF_BYTE);
    informationHeader[10] = static_cast<unsigned char>(height >> SHIFT_IN_THIRD_BYTE % MAX_VALUE_OF_BYTE);
    informationHeader[11] = static_cast<unsigned char>(height >> SHIFT_IN_FOURTH_BYTE % MAX_VALUE_OF_BYTE);
}

void Image::Export(const char* path, uint64_t*& pile, uint64_t new_width, uint64_t new_height) {
    width = new_width;
    height = new_height;

    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    const auto paddingAmount =
            static_cast<short>((SIZE_OF_COLOR - (width + width % 2) / 2 % SIZE_OF_COLOR) % SIZE_OF_COLOR);

    UpdateSize();

    file.write(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE);
    file.write(reinterpret_cast<char*>(informationHeader), INFORMATION_HEADER_SIZE);
    file.write(reinterpret_cast<char*>(Palette), PALETTE_SIZE);

    for (uint64_t y = height - 1; y != NEGATIVE_ONE; --y) {
        for (uint64_t x = 0; x < width; ++++x) {
            uint64_t num_of_grans_1 = pile[y * width + x + 1];
            uint64_t num_of_grans_2 = pile[y * width + x];
            uint64_t byte_1 = num_of_grans_1 < num_of_colors ? num_of_grans_1 : num_of_colors;
            uint64_t byte_2 = num_of_grans_2 < num_of_colors ?
                    num_of_grans_2 << 4 : num_of_colors << MAX_NUM_OF_GRANS_IN_CELL;

            file << static_cast<unsigned char>(byte_1 + byte_2);
        }

        file.write(zero, paddingAmount);
    }

    file.close();
}
