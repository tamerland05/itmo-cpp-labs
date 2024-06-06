#include "hamming_code.h"

uint8_t GetXor(const uint8_t* input, int degree) {
    uint8_t res = input[degree - 1];

    for (int c = 0; c < LengthOfEBlock; ++c) {
        if ((c + 1) % (degree * 2) >= degree) {
            res ^= input[c];
        }
    }

    return res;
}

void Encode(const uint8_t* input, uint8_t* output) {
    int degree = 1;

    for (int i = 0, j = 0; i < LengthOfEBlock; ++i, ++j) {
        if (i != degree - 1) {
            output[i] = input[j];
        } else {
            degree *= 2;
            --j;
        }
    }

    for (int i = 1; i < LengthOfEBlock; i *= 2) {
        output[i - 1] = GetXor(output, i);
    }
}

void Decode(uint8_t* input, uint8_t* output) {
    int degree = 1;
    int error = -1;

    for (int i = 1; i < LengthOfEBlock; i *= 2) {
        if (input[i - 1] != GetXor(input, i)) {
            error += i;
        }
    }

    if (error != -1) {
        if (error < LengthOfEBlock) {
            input[error] ^= 1;
        } else {
            std::cerr << "Error is not incorrigible.";
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0, j = 0; i < LengthOfEBlock; ++i, ++j) {
        if (i != degree - 1) {
            output[j] = input[i];
        } else {
            degree *= 2;
            --j;
        }
    }
}

void HammingCoding(const uint8_t* input, uint8_t* output, bool f) {
    uint8_t info[LengthOfIBlock] = {0};
    uint8_t encode[LengthOfEBlock] = {0};

    if (f) {
        GetBits(input, info, LengthOfIBlock);
        Encode(info, encode);
        ToBytes(encode, output, LengthOfEBlock);
    } else {
        GetBits(input, encode, LengthOfEBlock);
        Decode(encode, info);
        ToBytes(info, output, LengthOfIBlock);
    }
}

void GetBits(const uint8_t* input, uint8_t* output, int length) {
    int p_block = 0;

    for (int i = 0; i < length / SizeOfByte; ++i) {
        for (int cur_bit = SizeOfByte; cur_bit > 0; --cur_bit) {
            output[p_block++] = input[i] % (1 << cur_bit) >> (cur_bit - 1);
        }
    }
}

void ToBytes(const uint8_t* input, uint8_t* output, int length) {
    int p_block = 0;

    for (int i = 0; i < length / SizeOfByte; ++i) {
        for (int cur_bit = SizeOfByte - 1; cur_bit >= 0; --cur_bit) {
            output[i] += input[p_block++] << cur_bit;
        }
    }
}

uint64_t GetHammingLength(const uint64_t info_length) {
    return (info_length * 8 + (LengthOfEBlock - 1)) / LengthOfEBlock;
}
