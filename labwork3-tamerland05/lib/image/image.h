#include <fstream>
#include <cstdint>

const uint64_t FILE_HEADER_SIZE = 14;
const uint64_t INFORMATION_HEADER_SIZE = 40;
const uint64_t PALETTE_SIZE = 64;
const uint64_t NEGATIVE_ONE = static_cast<uint64_t>(-1);

const uint64_t SIZE_OF_UINT64_T = 8;
const uint64_t SIZE_OF_COLOR = 4;
const uint64_t MAX_NUM_OF_GRANS_IN_CELL = 4;

const uint64_t MAX_VALUE_OF_BYTE = 256;
const uint64_t SHIFT_IN_SECOND_BYTE = 8;
const uint64_t SHIFT_IN_THIRD_BYTE = 16;
const uint64_t SHIFT_IN_FOURTH_BYTE = 24;

struct Image {
    explicit Image();

    void Export(const char* path, uint64_t*& pile, uint64_t new_width, uint64_t new_height);
    void UpdateSize();

    uint64_t width{};
    uint64_t height{};
    uint64_t num_of_colors = 4;

    unsigned char fileHeader[FILE_HEADER_SIZE] = {0};
    unsigned char informationHeader[INFORMATION_HEADER_SIZE] = {0};
    unsigned char Palette [PALETTE_SIZE / 4][PALETTE_SIZE / 16] = {{0}};
    const char zero[3] = {0, 0, 0};
};
