#include "../lib/archiver.cpp"
#include <iostream>

int main(int argc, char* argv[]) {

    Archiver my_arch{};
    my_arch.Parse(argc, argv);

    my_arch.Do();

    return EXIT_SUCCESS;
}