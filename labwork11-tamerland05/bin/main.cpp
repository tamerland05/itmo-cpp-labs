#include "lib/search_engine.h"
#include "lib/argparser/ArgParser.h"

int main(int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::string> request;

    ArgumentParser::ArgParser parser("Parser for searcher");
    parser.AddHelp('h', "help");
    parser.AddStringArgument('p', "path", "Path of searching directory");
    parser.AddStringArgument('m', "mode", "Mode of working (indexing | searching)").Default("indexing");
    parser.AddStringArgument('o', "options", "Options of searching, using with mode \"searching\"")\
    .Default("").Positional().MultiValue().StoreValues(request);

    parser.Parse(std::vector<std::string>(argv, argv + argc));

    if (!std::filesystem::is_directory(parser.GetStringValue("path"))) {
        std::cerr << "Please, input correct path of directory, try --help for more.\n";
        system("pause");
        exit(EXIT_FAILURE);
    }
    SearchEngine search_engine(parser.GetStringValue("path"));

    if (parser.GetStringValue("mode") == "indexing") {
        search_engine.DoIndex();
    } else if (parser.GetStringValue("mode") == "searching") {
        search_engine.DoSearch(request);
    } else {
        std::cerr << "Please, input correct mode of working, try --help for more.\n";
        system("pause");
        exit(EXIT_FAILURE);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return 0;
}
