#include <cstring>
#include <charconv>
#include <stdexcept>
#include <cstdint>

const uint64_t LEN_OF_PARAM_INPUT_F = 8;
const uint64_t LEN_OF_PARAM_OUTPUT_F = 9;
const uint64_t LEN_OF_PARAM_MAX_I = 11;
const uint64_t LEN_OF_PARAM_FREQ = 7;
const uint64_t LEN_OF_PARAM_HELP = 6;

const uint64_t LEN_OF_SHORT_PARAM = 2;

typedef const std::invalid_argument e;

struct Arguments {
    char* input_file{}; //-i, --input - tsv-файл (tab-separated values) c начальными данными

    char* output_directory{}; //-o, --output - путь к директории для сохранения картинок

    int64_t max_iter{}; //-m, --max-iter - максимальное количество итераций модели

    int64_t frequency{}; //-f, --freq - частота, с которой должны сохранятся картинки (если 0, то сохраняется только последнее состояние)
};

struct Throws {
    e incorrect_num = e("Error: you should input the integer not letter or symbol. Try --help.");
    e void_after_i = e("You should input the name of input file after -i. Try --help");
    e void_name = e("You should input the name of input file after '='. Try --help");
    e void_after_o = e("You should input the path of output directory after -o. Try --help");
    e void_path = e("You should input the path of output directory after '='. Try --help");
    e void_after_m = e("You should input the num of maximum iterations after -m. Try --help");
    e void_max_i = e("You should input the num of maximum iterations after '='. Try --help");
    e void_after_f = e("You should input the num of frequency saves after -f. Try --help");
    e void_freq = e("You should input the num of frequency saves after '='. Try --help");

    e negative_max_i = e("Error: num of max iterations must be positive. Try --help.");
    e negative_freq = e("Error: num of frequency must be positive. Try --help.");

    e void_input_f = e("You should input the name of input file. Try --help");
    e void_output_p = e("You should input the path of output directory. Try --help");
    e void_iterations = e("You should input the num of maximum iterations. Try --help");
    e void_frequency = e("You should input the num of frequency saves. Try --help");
};

Arguments Parse(int argc, const char* argv[]);
