#include "../headers/matrix.hpp"

Matrix::Matrix(): num_row(0), num_column(0) 
{
    matrix = nullptr;
};

Matrix::Matrix(uint64_t size): num_row(size), num_column(size) 
{
    matrix = new type*[num_row];
    for (uint64_t i = 0; i < num_row; i++) {
        matrix[i] = new type[num_column];
    }
}

Matrix::Matrix(uint64_t num_row, uint64_t num_column): num_row(num_row), num_column(num_column) 
{
    matrix = new type*[num_row];
    for (uint64_t i = 0; i < num_row; i++) {
        matrix[i] = new type[num_column];
    }
}

Matrix::Matrix(const Matrix& other)
{
    this->num_row = other.num_row;
    this->num_column = other.num_column;
    matrix = new type*[num_row];
    
    for (uint64_t i = 0; i < num_row; i++) {
        matrix[i] = new type[num_column];
        for (uint64_t j = 0; j < num_column; j++) {
            matrix[i][j] = other.matrix[i][j];
        }
    }
}