#include "wrap.h"

Wrap::Wrap(uint8_t* array, size_t index, size_t num_of_columns)
: array_(array)
, index_(index)
, num_of_columns_(num_of_columns) {}

Wrap& Wrap::operator=(int value) {
    if (num_of_columns_) {
        std::cerr << "You should input all three indexes";
        exit(EXIT_FAILURE);
    }
    array_[first_uint8_t_] = (array_[first_uint8_t_] >> first_uint8_t_bit_ << first_uint8_t_bit_) +
                             (value >> (BitesInNum - first_uint8_t_bit_));

    array_[second_uint8_t_] = (value % (1 << (BitesInNum - first_uint8_t_bit_))) >> third_uint8_t_bit_;

    array_[third_uint8_t_] = (array_[third_uint8_t_] % (1 << (BitesInByte - third_uint8_t_bit_))) +
                             ((value % (1 << third_uint8_t_bit_)) << (BitesInByte - third_uint8_t_bit_));

    return *this;
}

Wrap::operator int() const {
    int int_value;

    int first_peace = (array_[first_uint8_t_] % (1 << first_uint8_t_bit_));
    int second_peace = array_[second_uint8_t_];
    int third_peace = (array_[third_uint8_t_] >> (BitesInByte - third_uint8_t_bit_));

    int_value = (first_peace << (BitesInNum - first_uint8_t_bit_))
                + (second_peace << third_uint8_t_bit_)
                + third_peace;

    return int_value;
}

Wrap& Wrap::operator[](size_t index) {
    if (num_of_columns_) {
        index_ += index * num_of_columns_;
        num_of_columns_ = 0;
    } else {
        index_ += index;

        cur_bit_ = index_ * BitesInNum;
        first_uint8_t_ = cur_bit_ / BitesInByte;
        first_uint8_t_bit_ = static_cast<uint8_t>(BitesInByte - cur_bit_ % BitesInByte);
        second_uint8_t_ = first_uint8_t_ + 1;
        third_uint8_t_ = second_uint8_t_ + 1;
        third_uint8_t_bit_ = static_cast<uint8_t>(BitesInFirstAndThirdBytes - first_uint8_t_bit_);
    }

    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Wrap& val) {
    stream << static_cast<int>(val);

    return stream;
}

std::istream& operator>>(std::istream& stream, Wrap& val) {
    int int_val;
    stream >> int_val;
    val = int_val;

    return stream;
}
