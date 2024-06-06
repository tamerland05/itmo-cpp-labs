#include <lib/hamming_code.h>
#include <gtest/gtest.h>

class HammingKodBitesTestSuite : public testing::TestWithParam<std::tuple<std::string, std::string>> {
};

TEST_P(HammingKodBitesTestSuite, CorrectTest) {
    uint8_t cor_inf[LengthOfIBlock] = {0};
    uint8_t cor_enc[LengthOfEBlock] = {0};
    int p{};
    for (auto i : std::get<0>(GetParam())) {
        cor_inf[p++] = i - 48;
    }
    p = 0;
    for (auto i : std::get<1>(GetParam())) {
        cor_enc[p++] = i - 48;
    }
    uint8_t test_enc[LengthOfEBlock] = {0};
    uint8_t test_inf[LengthOfIBlock] = {0};

    Encode(cor_inf, test_enc);
    bool f = true;
    for (int i = 0; i < LengthOfEBlock; ++i) {
        if (cor_enc[i] != test_enc[i]) {
            f = false;
            break;
        }
    }
    ASSERT_TRUE(f);

    Decode(cor_enc, test_inf);
    f = true;
    for (int i = 0; i < LengthOfIBlock; ++i) {
        if (cor_inf[i] != test_inf[i]) {
            f = false;
            break;
        }
    }
    ASSERT_TRUE(f);
}

INSTANTIATE_TEST_SUITE_P(
        Group,
        HammingKodBitesTestSuite,
        testing::Values(
                std::make_tuple("010111011", "0000101011011"),
                std::make_tuple("110100010010", "01101010000100100"),
                std::make_tuple("101011100111001011001", "01100100111001111001011001"),
                std::make_tuple("011011010110000101101101011000010010000001101101011010010110110001100001001000000111001001100001011011010111010100000000",
                                "1101110111010110000010110110101110000100100000011011010110100101110110001100001001000000111001001100001011011010111010100000000")
        )
);

class HammingKodBytesTestSuite : public testing::TestWithParam<std::tuple<std::string>> {
};

TEST_P(HammingKodBytesTestSuite, CorrectTest) {
    uint8_t cor_inf[LengthOfIBlock / 8] = {0};
    uint8_t cor_enc[LengthOfEBlock / 8] = {0};
    int p{};
    for (int i = 0; i < 19; ++i) {
        cor_inf[p++] = "\0\0\0\0\0\0\17\0\0\0\340\206\t.ia.zip"[i];//std::get<0>(GetParam())[i];
    }

    uint8_t test_enc[LengthOfEBlock / 8] = {0};
    uint8_t test_inf[LengthOfIBlock / 8] = {0};

    HammingCoding(cor_inf, test_enc);
    for (int i = 0; i < LengthOfEBlock / 8; ++i) {
        std::cout << (int)test_enc[i] << ' ';
    }

    HammingCoding(test_enc, test_inf, false);

    bool f = true;
    for (int i = 0; i < LengthOfIBlock / 8; ++i) {
        if (cor_inf[i] != test_inf[i]) {
            f = false;
            break;
        }
    }
    ASSERT_TRUE(f);
}

INSTANTIATE_TEST_SUITE_P(
        Group,
        HammingKodBytesTestSuite,
        testing::Values(
//                std::make_tuple("mama mila ramu"),
                std::make_tuple("\0\0\0\0\0\0\17\0\0\0\340\206\t.ia.zip")
        )
);
