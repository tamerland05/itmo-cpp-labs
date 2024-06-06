#include "lib/triple_array.h"

int main(int argc, const char* argv[]) {
    TripleArray arr(2, 2, 2);
    TripleArray arr2(2,2,2);
    TripleArray arr3(2,2,2);

    arr[1][1][1] = 10000;
    arr2[1][1][1] = 10000;
    std::cout << arr[1][1][1] << ' ' <<arr2[1][1][1] << '\n';
    arr3 = arr+arr2;
    std::cout << arr3 << '\n';
    std::cout << arr3[1][1][1];
    return 0;
}
