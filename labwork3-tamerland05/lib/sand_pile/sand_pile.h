#include "../parser/parser.h"
#include "../image/image.h"

#include <fstream>

const uint64_t LEN_OF_MAX_PATH = 120;

struct Sand_pile {
    explicit Sand_pile(Image& my_image);
    void UpdatePile();
    ~Sand_pile();

    void GoIterations(Arguments& arguments);
    void GetPile(char* output_directory, int64_t& iteration);
    void ParseInputData(char* filename);
    [[nodiscard]] bool IsPileState() const;

    uint64_t width{};
    uint64_t height{};
    Image my_image{};

    uint64_t* pile = nullptr;
};
