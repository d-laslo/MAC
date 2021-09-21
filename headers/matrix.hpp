#pragma once
#include "defines.hpp"
#include "source.hpp"
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <vector>


using type = uint64_t;

class Matrix
{
private:
    std::vector<std::vector<type>> matrix;
    uint64_t num_row;
    uint64_t num_column;

    void matrix_init();
public:
    Matrix();
    Matrix(uint64_t size);
    Matrix(uint64_t num_row, uint64_t num_column);
    Matrix(const Matrix& other);
    //Matrix(Matrix&& other);
    ~Matrix();

    std::vector<std::vector<type>>::iterator begin();
    std::vector<std::vector<type>>::iterator end();

    //transpose the matrix
    Matrix& T() const;

    Matrix& operator+(const Matrix& other) const;
    Matrix& operator*(const Matrix& other) const;
    std::vector<type>& operator[](uint64_t index);
};