#include "sand_pile.h"

uint64_t GetIntLen(uint64_t n) {
    uint64_t len = 0;

    while (n > 0) {
        ++len;
        n /= 10;
    }

    return len;
}

void CreatePath(int64_t& n, char* directory, char* path) {
    const uint64_t len = GetIntLen(static_cast<uint64_t>(n));
    char* name = new char[len];

    std::to_chars(name, name+len, n);
    const uint64_t path_len = std::strlen(directory);

    for (uint64_t i = 0; i < path_len; ++i) {
        path[i] = directory[i];
    }
    for (uint64_t i = path_len; i < path_len + len; ++i) {
        path[i] = name[i-path_len];
    }

    path[path_len + len] = '.';
    path[path_len + len + 1] = 'b';
    path[path_len + len + 2] = 'm';
    path[path_len + len + 3] = 'p';

    delete[] name;
}

Sand_pile::Sand_pile(Image &my_image) : my_image(my_image){}

Sand_pile::~Sand_pile() {
    delete[] pile;
}

void Sand_pile::UpdatePile() {
    uint64_t new_width = width;
    uint64_t new_height = height;

    for (uint64_t y = 0; y < height; ++y) {
        if (pile[y * width] >= MAX_NUM_OF_GRANS_IN_CELL) {
            ++new_width;
            break;
        }
    }
    const uint64_t diff_width = new_width - width;

    for (uint64_t y = 0; y < height; ++y) {
        if (pile[(y + 1) * width - 1] >= MAX_NUM_OF_GRANS_IN_CELL) {
            ++new_width;
            break;
        }
    }

    for (uint64_t x = 0; x < width; ++x) {
        if (pile[x] >= MAX_NUM_OF_GRANS_IN_CELL) {
            ++new_height;
            break;
        }
    }
    const uint64_t diff_height = new_height - height;

    for (uint64_t x = 0; x < width; ++x) {
        if (pile[(height - 1) * width + x] >= MAX_NUM_OF_GRANS_IN_CELL) {
            ++new_height;
            break;
        }
    }

    auto* new_pile = new uint64_t[new_width * new_height];
    memset(new_pile, 0, new_width * new_height * SIZE_OF_UINT64_T);

    for (uint64_t y = 0; y < height; ++y) {
        for (uint64_t x = 0; x < width; ++x) {
            uint64_t num_of_grans = pile[y * width + x];
            uint64_t new_x = x + diff_width;
            uint64_t new_y = y + diff_height;
            if (num_of_grans >= MAX_NUM_OF_GRANS_IN_CELL) {
                ++new_pile[new_x + 1 + new_y * new_width];
                ++new_pile[new_x - 1 + new_y * new_width];
                ++new_pile[new_x + (new_y - 1) * new_width];
                ++new_pile[new_x + (new_y + 1) * new_width];
                num_of_grans -= MAX_NUM_OF_GRANS_IN_CELL;
            }

            new_pile[new_x + new_y * new_width] += num_of_grans;
        }
    }

    width = new_width;
    height = new_height;

    delete[] pile;
    pile = new_pile;
}

void Sand_pile::GoIterations(Arguments& arguments) {
    int64_t iteration = 1;

    ParseInputData(arguments.input_file);

    while (iteration <= arguments.max_iter && !IsPileState()) {
        UpdatePile();

        if (arguments.frequency != 0 && (iteration - 1) % arguments.frequency == 0) {
            GetPile(arguments.output_directory, iteration);
        }

        ++iteration;
    }

    if (arguments.frequency == 0) {
        GetPile(arguments.output_directory, iteration);
    }

    delete[] arguments.output_directory;
}

void Sand_pile::GetPile(char* output_directory, int64_t& iteration) {
    char path[LEN_OF_MAX_PATH] = {0};
    CreatePath(iteration, output_directory, path);
    my_image.Export(path, pile, width, height);
}

void Sand_pile::ParseInputData(char* filename) {
    std::ifstream file;
    file.open(filename);

    if (!file) {
        printf("The %s file does not exist", filename);
        exit(1);
    }

    uint16_t tmp_x;
    uint16_t tmp_y;
    uint64_t tmp_grans;

    while (!file.eof()) {
        file >> tmp_x;
        if (tmp_x > width) {
            width = tmp_x;
        }
        file >> tmp_y;
        if (tmp_y > height) {
            height = tmp_y;
        }
        file >> tmp_grans;
    }

    delete[] pile;
    pile = new uint64_t[height * width];
    memset(pile, 0, height * width * SIZE_OF_UINT64_T);
    file.close();

    file.open(filename);

    while (!file.eof()) {
        file >> tmp_x;
        file >> tmp_y;
        file >> pile[(tmp_y - 1) * width + (tmp_x - 1)];
    }

    if (file.exceptions()) {
        printf("Error while reading the file.");
        exit(1);
    }

    file.close();

    delete[] filename;
}

bool Sand_pile::IsPileState() const {
    const uint64_t len = width * height;

    for (uint64_t i = 0; i < len; ++i) {
        if (pile[i] >= MAX_NUM_OF_GRANS_IN_CELL) {
            return false;
        }
    }

    return true;
}

