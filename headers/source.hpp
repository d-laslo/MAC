#pragma once
#include "defines.hpp"
#include "long_int.hpp"
#include <string>
#include <vector>


template<class T>
uint64_t high_bit(T value) 
{
    auto hb = sizeof(value) * 8 - 1;
    for (; hb >= 0 && hb < -1; hb--) {
        if ((value >> hb) & 0x1) {
            return hb + 1;
        }
    }
    return 0;
};

std::vector<Long>& transpose(const std::vector<Long>& m);
std::vector<Long>& concatenation(const std::vector<Long>& first, const std::vector<Long>& second);