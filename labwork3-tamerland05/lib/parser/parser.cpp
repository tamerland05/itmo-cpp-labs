#include "parser.h"

Throws throws;

void GetHelp(const char* path){
    printf("usage: %s", path);
    printf("Options and arguments:\n"
         "-i, --input       : tsv file (tab-separated values) with initial data\n"
         "-o  --output      : the path to the directory for saving images\n"
         "-h  --help        : give a short information about program\n"
         "-m  --max-iter    : maximum number of iterations of the model\n"
         "-f  --freq        : the frequency with which the images should be saved (if 0, then only the last state is saved)\n"
         "--------------------\n"
         "The file name and options are passed via command line arguments in the following format:\n"
         "labwork3.exe [OPTION] input_filename");
}

int64_t GetNum(const char* string, const int start=0) {
    int64_t value;
    const uint64_t len = strlen(string);
    const std::from_chars_result res = std::from_chars(&string[start], &string[len], value);

    if (res.ec == std::errc{}){
        return value;
    }

    throw throws.incorrect_num;
}

Arguments Parse(int argc, const char* argv[]){
    Arguments arguments;
    bool freq_flag = false;

    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-i", LEN_OF_SHORT_PARAM) == 0) {
            if (argc > i + 1) {
                ++i;
                std::copy(argv[i], (argv[i] + strlen(argv[i])), arguments.input_file);
            } else {
                throw throws.void_after_i;
            }
        } else if ((strncmp(argv[i], "--input", LEN_OF_PARAM_INPUT_F - 1) == 0
                   && strlen(argv[i]) == LEN_OF_PARAM_INPUT_F - 1) ||
                   (strncmp(argv[i], "--input=", LEN_OF_PARAM_INPUT_F) == 0
                   && strlen(argv[i]) == LEN_OF_PARAM_INPUT_F)) {
            throw throws.void_name;
        } else if (strncmp(argv[i], "--input=", LEN_OF_PARAM_INPUT_F) == 0) {
            uint64_t len_name = strlen(argv[i]) - LEN_OF_PARAM_INPUT_F;
            char* input_file = new char[len_name];
            for (uint64_t j = 0; j < len_name; ++j) {
                input_file[j] = argv[i][j + LEN_OF_PARAM_INPUT_F];
            }
            input_file[len_name] = '\0';
            arguments.input_file = input_file;
        } else if (strncmp(argv[i], "-o", LEN_OF_SHORT_PARAM) == 0) {
            if (argc > i + 1) {
                ++i;
                 std::copy(argv[i], (argv[i] + strlen(argv[i])), arguments.output_directory);
            }
            else {
                throw throws.void_after_o;
            }
        } else if ((strncmp(argv[i], "--output", LEN_OF_PARAM_OUTPUT_F - 1) == 0
                   && strlen(argv[i]) == LEN_OF_PARAM_OUTPUT_F - 1) ||
                   (strncmp(argv[i], "--output=", LEN_OF_PARAM_OUTPUT_F) == 0
                   && strlen(argv[i]) == LEN_OF_PARAM_OUTPUT_F)) {
            throw throws.void_path;
        } else if (strncmp(argv[i], "--output=", LEN_OF_PARAM_OUTPUT_F) == 0) {
            uint64_t len_path = strlen(argv[i]) - LEN_OF_PARAM_OUTPUT_F;
            char* output_path = new char[len_path];
            for (uint64_t j = 0; j < len_path; ++j) {
                output_path[j] = argv[i][j + LEN_OF_PARAM_OUTPUT_F];
            }
            arguments.output_directory = output_path;
        } else if (strncmp(argv[i], "-m", LEN_OF_SHORT_PARAM) == 0) {
            if (argc > i + 1) {
                ++i;
                arguments.max_iter = GetNum(argv[i]);
            }
            else {
                throw throws.void_after_m;
            }
        } else if ((strncmp(argv[i], "--max-iter", LEN_OF_PARAM_MAX_I - 1) == 0
                   && strlen(argv[i]) == LEN_OF_PARAM_MAX_I - 1) ||
                   (strncmp(argv[i], "--max-iter=", LEN_OF_PARAM_MAX_I) == 0
                   && strlen(argv[i]) == LEN_OF_PARAM_MAX_I)) {
            throw throws.void_max_i;
        } else if (strncmp(argv[i], "--max-iter=", LEN_OF_PARAM_MAX_I) == 0) {
            arguments.max_iter = GetNum(argv[i], LEN_OF_PARAM_MAX_I);
        } else if (strncmp(argv[i], "-f", LEN_OF_SHORT_PARAM) == 0) {
            if (argc > i + 1) {
                ++i;
                arguments.frequency = GetNum(argv[i]);
                freq_flag = true;
            }
            else {
                throw throws.void_after_f;
            }
        } else if ((strncmp(argv[i], "--freq", LEN_OF_PARAM_FREQ - 1) == 0
                   && strlen(argv[i]) == LEN_OF_PARAM_FREQ - 1) ||
                   (strncmp(argv[i], "--freq=", LEN_OF_PARAM_FREQ) == 0 && strlen(argv[i]) == 7)) {
            throw throws.void_freq;
        } else if (strncmp(argv[i], "--freq=", LEN_OF_PARAM_FREQ) == 0) {
            arguments.frequency = GetNum(argv[i], LEN_OF_PARAM_FREQ);
            freq_flag = true;
        } else if (strncmp(argv[i], "-h", LEN_OF_SHORT_PARAM) == 0 ||
                   strncmp(argv[i], "--help", LEN_OF_PARAM_HELP) == 0) {
            GetHelp(argv[0]);
            std::exit(0);
        }
    }


    if (arguments.frequency < 0) {
        throw throws.negative_freq;
    }
    if (arguments.max_iter < 0) {
        throw throws.negative_max_i;
    }

    if (!arguments.input_file) {
        throw throws.void_input_f;
    } else if (!arguments.output_directory) {
        throw throws.void_output_p;
    } else if (!arguments.max_iter) {
        throw throws.void_iterations;
    } else if (!freq_flag) {
        throw throws.void_frequency;
    } else {
        return arguments;
    }
}
