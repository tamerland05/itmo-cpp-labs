#include "wrap.h"

class TripleArray {
public:
    explicit TripleArray(size_t width, size_t height, size_t length);
    TripleArray(const TripleArray& copy_array);
    ~TripleArray();

    static TripleArray make_array(size_t num_of_tables, size_t num_of_rows, size_t num_of_columns);

    TripleArray& operator=(const TripleArray& copy_array);
    Wrap operator[](size_t table);

    TripleArray operator*(int multi);
    TripleArray operator+(TripleArray& rhs);
    TripleArray operator-(TripleArray& rhs);

    friend std::ostream& operator<<(std::ostream& stream, TripleArray& array);

private:
    [[nodiscard]] TripleArray Calculate(char op, int multi, TripleArray& rhs);

    [[maybe_unused]] static int counter;
    uint8_t* array_;
    size_t width_;
    size_t height_;
    size_t length_;

};
std::ostream& operator<<(std::ostream& stream, const TripleArray& array);

int TripleArray:: counter = 0;
void Foo() {
    static int tmp{};
}
int tmp = 0;
