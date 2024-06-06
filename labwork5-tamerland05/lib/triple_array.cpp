#pragma once
#include "triple_array.h"

TripleArray::TripleArray(size_t width, size_t height, size_t length)
: width_(width)
, height_(height)
, length_(length) {
    size_t bit_size = width * height * length * BitesInNum;
    size_t size = (bit_size + BitesInByte - 1) / BitesInByte;
    array_ = new uint8_t[size] {0};
}

TripleArray::TripleArray(const TripleArray& copy_arr) {
    length_ = copy_arr.length_;
    height_ = copy_arr.height_;
    width_ = copy_arr.width_;

    array_ = new uint8_t[length_ * height_ * width_];
    std::copy(copy_arr.array_,
              copy_arr.array_ + length_ * height_ * width_,
              array_);
}

TripleArray& TripleArray::operator=(const TripleArray& copy_arr) {
    if (this == &copy_arr) {
        return *this;
    }
    delete[] array_;

    length_ = copy_arr.length_;
    height_ = copy_arr.height_;
    width_ = copy_arr.width_;

    array_ = new uint8_t[length_ * height_ * width_];
    std::copy(copy_arr.array_,
              copy_arr.array_ + (length_ * height_ * width_ * 17 + BitesInByte - 1) / 8,
              array_);

    return *this;
}

TripleArray::~TripleArray() {
    delete[] array_;
}

TripleArray TripleArray::make_array(size_t num_of_tables, size_t num_of_rows, size_t num_of_columns) {
    return TripleArray{num_of_tables, num_of_rows, num_of_columns};
}

Wrap TripleArray::operator[](size_t table) {
    return {array_, table * height_ * length_, length_};
}

TripleArray TripleArray::operator*(int multi) {
    return Calculate('*', multi, *this);
}

TripleArray TripleArray::operator+(TripleArray& rhs) {
    return Calculate('+', 0, rhs);
}

TripleArray TripleArray::operator-(TripleArray& rhs) {
    return Calculate('-', 0, rhs);
}

TripleArray TripleArray::Calculate(char op, int multi, TripleArray& rhs) {
    TripleArray new_array =
            make_array(width_, height_, length_);

    for (size_t x = 0; x < width_; ++x) {
        for (size_t y = 0; y < height_; ++y) {
            for (size_t z = 0; z < length_; ++z) {
                int left = static_cast<int>((*this)[x][y][z]);
                int right = op == '*' ? multi : static_cast<int>(rhs[x][y][z]);
                new_array[x][y][z] = op == '*' ? left * right : (op == '+' ? left + right : left - right);
            }
        }
    }

    return new_array;
}

std::ostream& operator<<(std::ostream& stream, TripleArray& array) {
    for (size_t x = 0; x < array.width_; ++x) {
        for (size_t y = 0; y < array.height_; ++y) {
            for (size_t z = 0; z < array.length_; ++z) {
                stream << array[x][y][z] << ' ';
            }
        }
    }

    return stream;
}
