#include "number.h"
#include <cstring>
#include <charconv>

const uint8_t LAST_BYTE = 252;
const uint8_t SIGN_BIT = 127;
const uint8_t ASCII_SHIFT = 48;
const uint16_t MAX_NUM_LEN = 610;

void IfOperandNegative(int2023_t& i, bool& f) {
    if (i._value[LAST_BYTE] > SIGN_BIT) {
        f = !(f);
        i = -i;
    }
}

int GetLen(const int2023_t& i) {
    int16_t bytes = 252;
    int16_t bites = 7;

    while (bytes >= 0 && i._value[bytes] == 0) {
        --bytes;
    }
    while ((i._value[bytes] >> bites) < 1) {
        --bites;
    }

    return bytes * 8 + bites + 1;
}

bool IsNull(const uint8_t* buff_of_nums, uint64_t buff_len) {
    uint16_t bites = 0;

    while (bites < buff_len && *(buff_of_nums + bites) == 0) {
        ++bites;
    }

    return bites == buff_len;
}

void RecAdd(uint8_t byte, int buff_of_add, int2023_t& res) {
    if (res._value[byte] + buff_of_add > 255) {
        RecAdd(byte + 1, (res._value[byte] + buff_of_add) / 256, res);
    }

    res._value[byte] = (res._value[byte] + buff_of_add) % 256;
}

int2023_t from_int(int32_t i) {
    int2023_t res;
    bool is_res_negative = i < 0;

    if (is_res_negative) {
        i *= -1 ;
    }

    for (int j = 3; j >= 0; --j) {
        res._value[j] = i >> j * 8;
        i %= 1 << j * 8;
    }

    return is_res_negative ? -res : res;
}

int2023_t from_string(const char* buff) {
    int2023_t res;
    uint64_t len = std::strlen(buff);
    int16_t bites = 0;
    int16_t negative_flag = 0;
    uint8_t copy_of_buff[MAX_NUM_LEN] = {0};
    bool is_res_negative = buff[0] == '-';

    if (is_res_negative) {
        ++negative_flag;
    }

    for (int j = 0; j < len - negative_flag; ++j) {
        copy_of_buff[j] = static_cast<uint8_t>(buff[len - j - 1] - ASCII_SHIFT);
    }

    while (!IsNull(copy_of_buff, len)) {
        res._value[bites / 8] += (copy_of_buff[0] % 2) << (bites % 8); ++bites;
        copy_of_buff[0] >>= 1;
        for (int j = 1; j < len; ++j) {
            copy_of_buff[j - 1] += copy_of_buff[j] % 2 * 5;
            copy_of_buff[j] >>= 1;
        }
    }

    return is_res_negative ? -res : res;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t res;
    uint16_t buff = 0;
    uint8_t byte = 0;

    for (int i = 0; i < 253; ++i) {
        buff += static_cast<uint16_t>(rhs._value[i] + lhs._value[i]);

        res._value[i] += buff % 256;
        buff /= 256;
    }

    if (buff > 0) {
        while (res._value[byte] == 255) res._value[byte++] = 0;
        ++res._value[byte];
    }

    while (res._value[byte] == 255) {
        ++byte; // если получился минус ноль (все единицы)
    }

    return byte == 253 ? -res : res;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    return lhs + -rhs;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t res;
    int2023_t copy_l = lhs;
    int2023_t copy_r = rhs;
    uint16_t buff;
    bool is_res_negative = false;

    IfOperandNegative(copy_l, is_res_negative);
    IfOperandNegative(copy_r, is_res_negative);

    for (uint8_t i = 0; i <= LAST_BYTE; ++i) {
        for (uint8_t j = 0; j <= LAST_BYTE; ++j) {
            buff = copy_l._value[i] * copy_r._value[j];
            RecAdd(i + j, buff, res);
        }
    }

    return is_res_negative ? -res : res;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t res;
    int2023_t buff;
    int2023_t copy_l = lhs;
    int2023_t copy_r = rhs;
    bool is_res_negative = false;

    IfOperandNegative(copy_l, is_res_negative);
    IfOperandNegative(copy_r, is_res_negative);

    if (copy_l < copy_r) {
        return from_int(0);
    }

    for (int i = GetLen(copy_l) - 1; i >= 0; --i) {
        int i_div8 = i / 8, i_mod8 = i % 8;

        buff = buff * from_int(2);
        buff._value[0] += (copy_l._value[i_div8] % (1 << (i_mod8 + 1))) >> (i_mod8);

        if (!(buff < copy_r)) {
            buff = buff - copy_r;
            if (res._value[i_div8] % (1 << i_mod8) < 1 << i_mod8) {
                res._value[i_div8] += 1 << (i_mod8);
            }
        }
    }

    return is_res_negative ? -res : res;
}

int2023_t operator-(const int2023_t& i){
    int2023_t copy_i = i;

    for (unsigned char & j : copy_i._value) {
        j = ~j;
    }

    return copy_i;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    int16_t byte = LAST_BYTE;

    while (byte >= 0 && rhs._value[byte] == lhs._value[byte]) {
        --byte;
    }

    return (byte == -1);
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
    int16_t byte = 252;

    while (byte >= 0 && lhs._value[byte] == rhs._value[byte]) {
        --byte;
    }

    if (byte < 0) {
        return false;
    } else {
        return (lhs._value[byte] > rhs._value[byte]);
    }
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    return rhs > lhs;
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    char buff_for_bite[9] = {0};

    for (int len, i = 252; i >= 0; --i) {
        std::to_chars(buff_for_bite, buff_for_bite + 8, value._value[i], 2);

        const char* const_copy = static_cast<const char*>(buff_for_bite);
        len = static_cast<int>(8 - strlen(const_copy));
        for (int j = 0; j < len; ++j) {
            stream << '0';
        }

        stream << buff_for_bite;
        memset(&buff_for_bite[0], 0, 9);
    }

    return stream;
}
