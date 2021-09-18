#pragma once
#include "defines.hpp"
#include "source.hpp"
#include <cstdint>


using type = uint64_t;

class Matrix
{
private:
    type** matrix;
    uint64_t num_row;
    uint64_t num_column;
public:
    Matrix();
    Matrix(uint64_t size);
    Matrix(uint64_t num_row, uint64_t num_column);
    Matrix(const Matrix& other);
    //Matrix(Matrix&& other);
};