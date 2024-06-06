#pragma once

#include <cstdint>
#include <ostream>
#include <istream>
#include <iostream>

const uint8_t BitesInNum = 17;
const uint8_t BitesInByte = 8;
const uint8_t BitesInFirstAndThirdBytes = 9;
static int a{};

class Wrap{
public:
    Wrap(uint8_t* array, size_t index, size_t num_of_columns);
    Wrap& operator[](size_t index);
    Wrap& operator=(int value);
    explicit operator int() const;

private:
    uint8_t* array_;

    size_t index_;
    size_t num_of_columns_;

    size_t cur_bit_{};
    size_t first_uint8_t_{};
    uint8_t first_uint8_t_bit_{};
    size_t second_uint8_t_{};
    size_t third_uint8_t_{};
    uint8_t third_uint8_t_bit_{};
};

std::ostream& operator<<(std::ostream& stream, const Wrap& val);
std::istream& operator>>(std::istream& stream, Wrap& val);
