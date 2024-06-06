#include <lib/parser/parser.h>
#include <gtest/gtest.h>

Throws test_throws;

TEST(VoidAftITestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "-i"};

    try {
        Parse(2, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_after_i.what());
    }
}


TEST(VoidInput1TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input",
                          "--output=C:\\C++\\",
                          "--max-iter=1000",
                          "--freq=0"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_name.what());
    }
}

TEST(VoidInput2TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=",
                          "--output=C:\\C++\\",
                          "--max-iter=1000",
                          "--freq=0"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_name.what());
    }
}

TEST(VoidAftOTestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "-o"};

    try {
        Parse(3, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_after_o.what());
    }
}

TEST(VoidPath1TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output",
                          "--max-iter=1000",
                          "--freq=0"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_path.what());
    }
}

TEST(VoidPath2TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=",
                          "--max-iter=1000",
                          "--freq=0"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_path.what());
    }
}

TEST(VoidAftMTestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "-m"};

    try {
        Parse(4, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_after_m.what());
    }
}

TEST(VoidMaxI1TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter",
                          "--freq=0"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_max_i.what());
    }
}

TEST(VoidMaxI2TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=",
                          "--freq=0"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_max_i.what());
    }
}

TEST(VoidAftFTestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=1000",
                          "-f"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_after_f.what());
    }
}

TEST(VoidFreq1TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=1000",
                          "--freq"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_freq.what());
    }
}

TEST(VoidFreq2TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=1000",
                          "--freq="};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_freq.what());
    }
}

TEST(VoidIncNum1TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=abv",
                          "--freq=0"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.incorrect_num.what());
    }
}

TEST(VoidIncNum2TestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=1000",
                          "--freq=abv"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.incorrect_num.what());
    }
}

TEST(VoidInpFTestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--output=C:\\C++\\",
                          "--max-iter=1000",
                          "--freq=0"};

    try {
        Parse(4, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_input_f.what());
    }
}

TEST(VoidOutPTestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--max-iter=1000",
                          "--freq=0"};

    try {
        Parse(4, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_output_p.what());
    }
}

TEST(VoidIterTestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--freq=0"};

    try {
        Parse(4, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_iterations.what());
    }
}

TEST(VoidFreqTestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=1000"};

    try {
        Parse(4, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.void_frequency.what());
    }
}

TEST(NegITestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=-1000",
                          "--freq=1000"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.negative_max_i.what());
    }
}

TEST(NegFTestsSuite, EqualTest) {
    const char* argv[] = {"",
                          "--input=aboba.txt",
                          "--output=C:\\C++\\",
                          "--max-iter=1000",
                          "--freq=-1000"};

    try {
        Parse(5, argv);
    } catch (std::exception& ex) {
        ASSERT_STREQ(ex.what(), test_throws.negative_freq.what());
    }
}