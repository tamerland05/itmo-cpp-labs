#include <iostream>
#include <cstdint>

const int LengthOfEBlock = 1024;
const int LengthOfIBlock = 1014;
const uint8_t SizeOfByte = 8;

uint8_t GetXor(const uint8_t* input, int degree);

void Encode(const uint8_t* input, uint8_t* output);

void Decode(uint8_t* input, uint8_t* output);

void HammingCoding(const uint8_t* input, uint8_t* output, bool f=true); // по умолчанию мы кодируем

void GetBits(const uint8_t* input, uint8_t* output, int length);

void ToBytes(const uint8_t* input, uint8_t* output, int length);

uint64_t GetHammingLength(uint64_t info_length);