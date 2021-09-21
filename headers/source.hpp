#pragma once
#include "defines.hpp"
#include <string>

template<class T>
uint64_t high_bit(T value) 
{
    auto hb = sizeof(value) * 8 - 1;
    for (; hb >= 0; hb--) {
        if ((value >> hb) & 0x1) {
            return hb + 1;
        }
    }
    return 0;
}