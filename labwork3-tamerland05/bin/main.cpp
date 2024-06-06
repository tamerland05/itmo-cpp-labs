#include "lib/sand_pile/sand_pile.h"

int main(int argc, const char* argv[]) {
    Arguments arguments = Parse(argc, argv);

    Image image{};
    Sand_pile my_pile(image);

    my_pile.GoIterations(arguments);

    return 0;
}
