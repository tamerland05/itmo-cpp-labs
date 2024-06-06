#include "lib/triple_array.cpp"
#include "lib/wrap.cpp"

#include <gtest/gtest.h>

#include <random>
#include <tuple>

class ConvertingTestsSuite : public testing::TestWithParam<std::tuple<int32_t, int32_t, bool>> {
};

TEST_P(ConvertingTestsSuite, EqualTest) {
    TripleArray arr = TripleArray::make_array(18, 19, 20);
    arr[4][5][6] = std::get<0>(GetParam());
    int32_t a = static_cast<int>(arr[4][5][6]);
    int32_t b = std::get<1>(GetParam());

    if(std::get<2>(GetParam()))
        ASSERT_EQ(a,b) << std::get<0>(GetParam()) << " == " << std::get<1>(GetParam());
    else
        ASSERT_NE(a,b) << std::get<0>(GetParam()) << " != " << std::get<1>(GetParam());
}

INSTANTIATE_TEST_SUITE_P(
        Group,
        ConvertingTestsSuite,
        testing::Values(
                // positive
                std::make_tuple(0, 0, true),
                std::make_tuple(239,239, true),
                std::make_tuple(255,255, true),
                std::make_tuple(256,256, true),
                std::make_tuple(10000,10000, true),
                std::make_tuple(32767,32767, true),
                std::make_tuple(32768,32768, true),
                std::make_tuple(65535,65535, true),
                std::make_tuple(131071, 131071, true),

                // negative
                std::make_tuple(0, 1, false),
                std::make_tuple(32768,32769, false),
                std::make_tuple(255,256, false),
                std::make_tuple(256,255, false),
                std::make_tuple(31251,31252, false),
                std::make_tuple(131070,131071, false)
        )
);

class EqualTestSuite : public testing::TestWithParam<std::tuple<size_t, size_t, size_t, int>> {
};

TEST_P(EqualTestSuite, EqualTest) {
    std::random_device rd;
    std::mt19937 gen(rd());
    size_t tables = std::get<0>(GetParam());
    size_t rows = std::get<1>(GetParam());
    size_t columns = std::get<2>(GetParam());

    TripleArray a = TripleArray::make_array(tables, rows, columns);
    int* b = new int[200000000] {0};
    bool f = true;

    std::uniform_int_distribution<> distribution(1,131071);
    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                int val = static_cast<int>(distribution(gen));
                a[x][y][z] = val;
                b[x * rows * columns + y * columns + z] = val;
            }
        }
    }

    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                if (static_cast<int>(a[x][y][z]) != b[x * rows * columns + y * columns + z]) {
                    f = false;
                    break;
                }
            }
        }
    }

    delete[] b;
    ASSERT_TRUE(f);
}

TEST(TwoEqualArrays, EqualArrTest) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1,1000);
    auto tables = static_cast<size_t>(dist(gen));
    auto rows = static_cast<size_t>(dist(gen));
    auto columns = static_cast<size_t>(dist(gen));

    TripleArray a = TripleArray::make_array(tables, rows, columns);
    TripleArray b = TripleArray::make_array(tables, rows, columns);
    bool f = true;

    std::uniform_int_distribution<> distribution(1,131071);
    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                int val = static_cast<int>(distribution(gen));
                a[x][y][z] = val;
                b[x][y][z] = val;
            }
        }
    }

    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                if (static_cast<int>(a[x][y][z]) != static_cast<int>(b[x][y][z])) {
                    f = false;
                    break;
                }
            }
        }
    }

    ASSERT_TRUE(f);
}
TEST_P(EqualTestSuite, EqualMultiTest) {
    std::random_device rd;
    std::mt19937 gen(rd());
    size_t tables = std::get<0>(GetParam());
    size_t rows = std::get<1>(GetParam());
    size_t columns = std::get<2>(GetParam());
    int multi = std::get<3>(GetParam());

    TripleArray a = TripleArray::make_array(tables, rows, columns);
    int* c = new int[200000000] {0};
    bool f = true;

    std::uniform_int_distribution<> distribution(1,131);
    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                int val = static_cast<int>(distribution(gen));
                a[x][y][z] = val;
                c[x * rows * columns + y * columns + z] = val * multi;
            }
        }
    }

    TripleArray b = a * multi;
    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                if (static_cast<int>(b[x][y][z]) != c[x * rows * columns + y * columns + z]) {
                    f = false;
                    break;
                }
            }
        }
    }

    delete[] c;
    ASSERT_TRUE(f);
}

TEST_P(EqualTestSuite, EqualAddTest) {
    std::random_device rd;
    std::mt19937 gen(rd());
    size_t tables = std::get<0>(GetParam());
    size_t rows = std::get<1>(GetParam());
    size_t columns = std::get<2>(GetParam());

    TripleArray a = TripleArray::make_array(tables, rows, columns);
    TripleArray b = TripleArray::make_array(tables, rows, columns);
    int* c = new int[200000000] {0};
    bool f = true;

    std::uniform_int_distribution<> distribution(1,65000);
    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                int val1 = static_cast<int>(distribution(gen));
                int val2 = static_cast<int>(distribution(gen));
                a[x][y][z] = val1;
                b[x][y][z] = val2;
                c[x * rows * columns + y * columns + z] = val1 + val2;
            }
        }
    }

    TripleArray d = a + b;
    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                if (static_cast<int>(d[x][y][z]) != c[x * rows * columns + y * columns + z]) {
                    f = false;
                    break;
                }
            }
        }
    }

    delete[] c;
    ASSERT_TRUE(f);
}

TEST_P(EqualTestSuite, EqualSubTest) {
    std::random_device rd;
    std::mt19937 gen(rd());
    size_t tables = std::get<0>(GetParam());
    size_t rows = std::get<1>(GetParam());
    size_t columns = std::get<2>(GetParam());

    TripleArray a = TripleArray::make_array(tables, rows, columns);
    TripleArray b = TripleArray::make_array(tables, rows, columns);
    int* c = new int[200000000] {0};
    bool f = true;

    std::uniform_int_distribution<> distribution(1,65000);
    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                int val1 = static_cast<int>(distribution(gen));
                int val2 = static_cast<int>(distribution(gen));
                a[x][y][z] = std::max(val1, val2);
                b[x][y][z] = std::min(val1, val2);
                c[x * rows * columns + y * columns + z] = std::max(val1, val2) - std::min(val1, val2);
            }
        }
    }

    TripleArray d = a - b;
    for (size_t x = 0; x < tables; ++x) {
        for (size_t y = 0; y < rows; ++y) {
            for (size_t z = 0; z < columns; ++z) {
                if (static_cast<int>(d[x][y][z]) != c[x * rows * columns + y * columns + z]) {
                    f = false;
                    break;
                }
            }
        }
    }

    delete[] c;
    ASSERT_TRUE(f);
}

INSTANTIATE_TEST_SUITE_P(
        Group,
        EqualTestSuite,
        testing::Values(
                std::make_tuple(10, 11, 12, 123),
                std::make_tuple(100, 100, 100, 0),
                std::make_tuple(757, 125, 189, 444),
                std::make_tuple(876, 851, 216, 228),
                std::make_tuple(553, 112, 734, 1),
                std::make_tuple(473, 600, 426, 67),
                std::make_tuple(781, 585, 214, 900),
                std::make_tuple(363, 574, 553, 666),
                std::make_tuple(828, 111, 759, 2),
                std::make_tuple(211, 992, 671, 33),
                std::make_tuple(456, 698, 558, 444),
                std::make_tuple(634, 512, 435, 555)
        )
);

