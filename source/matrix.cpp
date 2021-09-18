#include "../headers/matrix.hpp"

Matrix::Matrix(): num_row(0), num_column(0) 
{
    matrix = nullptr;
};

Matrix::Matrix(uint64_t size): num_row(size), num_column(size) 
{
    matrix = matrix_init();
}

Matrix::Matrix(uint64_t num_row, uint64_t num_column): num_row(num_row), num_column(num_column) 
{
    matrix = matrix_init();
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

type** Matrix::matrix_init()
{
    type** new_matrix = new type*[num_row];
    for (uint64_t i = 0; i < num_row; i++) {
        matrix[i] = new type[num_column];
    }
    return new_matrix;
}

Matrix& Matrix::T() const 
{
    Matrix* transpose_matrix = new Matrix(num_column, num_row);

    for (uint64_t i = 0; i < num_row; i++) {
        for (uint64_t j = 0; j < num_column; j++) {
            transpose_matrix->matrix[j][i] = matrix[i][j];
        }
    }
    return *transpose_matrix;
}

Matrix& Matrix::operator+(const Matrix& other) const
{
    if ( (this->num_row != other.num_row) || (this->num_column != other.num_column) ) {
        throw -1;
    }

    Matrix* result = new Matrix(num_row, num_column);
    for (uint64_t i = 0; i < num_row; i++) {
        for (uint64_t j = 0; j < num_column; j++) {
            result->matrix[i][j] = this->matrix[i][j] + other.matrix[i][j];
        }
    }

    return *result;
}

Matrix& Matrix::operator*(const Matrix& other) const
{
    if (this->num_column != other.num_row) {
        throw -1;
    }

    auto tmp_matrix = other.T();
    Matrix* result = new Matrix(this->num_row, other.num_column);
    
    std::vector<type*> vectors;
    for (uint64_t i = 0; i < this->num_row; i++) {
        for (uint64_t j = 0; j < other.num_column; j++) {
            vectors[0] = this->matrix[i];
            vectors[1] = tmp_matrix.matrix[j];
            result->matrix[i][j] = std::accumulate(ALL(vectors), (type)0, 
                [](auto a, auto b)
                {
                    return a + b[0] * b[1];
                }
            );
        }
    }
}