#ifndef MATRIX_LIB_INCLUDE_MATRIX_H_
#define MATRIX_LIB_INCLUDE_MATRIX_H_

#include <stdlib.h>

#include <iostream>

template <typename Type, std::size_t RowSize, std::size_t ColSize>
class Matrix {
 public:
  Matrix() : row_size_(RowSize), col_size_(ColSize) {
    matrix_ = (Type **)malloc(row_size_ * sizeof(Type *));
    for (std::size_t r = 0; r < row_size_; r++) {
      matrix_[r] = (Type *)malloc(col_size_ * sizeof(Type));
    }
    SetZeros();
  }

  // Copy Constructor
  Matrix(const Matrix &other) : row_size_(RowSize), col_size_(ColSize) {
    matrix_ = (Type **)malloc(other.row_size_ * sizeof(Type *));
    for (std::size_t r = 0; r < row_size_; r++) {
      matrix_[r] = (Type *)malloc(other.col_size_ * sizeof(Type));
      for (std::size_t c = 0; c < col_size_; c++) {
        matrix_[r][c] = other.matrix_[r][c];
      }
    }
  }

  // Assignment Operator
  Matrix &operator=(const Matrix &other) = delete;

  // Addition
  Matrix operator+(const Matrix &other) {
    Matrix result;
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        result.matrix_[r][c] = matrix_[r][c] + other.matrix_[r][c];
      }
    }
    return result;
  }

  // Assign Addition
  Matrix &operator+=(const Matrix &other) {
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        matrix_[r][c] += other.matrix_[r][c];
      }
    }
    return *this;
  }

  // Subtraction
  Matrix operator-(const Matrix &other) {
    Matrix result;
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        result.matrix_[r][c] = matrix_[r][c] - other.matrix_[r][c];
      }
    }
    return result;
  }

  // Assign Subtraction
  Matrix &operator-=(const Matrix &other) {
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        matrix_[r][c] -= other.matrix_[r][c];
      }
    }
    return *this;
  }

  // Matrix Multiplication
  template <std::size_t N>
  Matrix<Type, RowSize, N> operator*(const Matrix<Type, ColSize, N> &other) {
    Matrix<Type, RowSize, N> result;
    for (std::size_t r = 0; r < result.row_size_; r++) {
      for (std::size_t c = 0; c < result.col_size_; c++) {
        for (std::size_t k = 0; k < col_size_; k++) {
          result.matrix_[r][c] += matrix_[r][k] * other.matrix_[k][c];
        }
      }
    }
    return result;
  }

  // Deconstructor
  ~Matrix() {
    for (std::size_t r = 0; r < row_size_; r++) {
      if (matrix_[r] != nullptr) {
        free(matrix_[r]);
        matrix_[r] = nullptr;
      }
    }
    if (matrix_ != nullptr) {
      free(matrix_);
      matrix_ = nullptr;
    }
  }

  void SetZeros() {
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        matrix_[r][c] = 0;
      }
    }
  }

  void SetOnes() {
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        matrix_[r][c] = 1;
      }
    }
  }

  Type At(std::size_t r, std::size_t c) const {
    CheckOutOfBound(r, c);
    return matrix_[r][c];
  }

  void SetValue(std::size_t r, std::size_t c, Type value) {
    CheckOutOfBound(r, c);
    matrix_[r][c] = value;
  }

  void Print() const {
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        std::cout << matrix_[r][c] << " ";
      }
      std::cout << std::endl;
    }
  }

  // TODO(jinwon.pyo): Make below private; you need to make operator* friend
  void CheckOutOfBound(std::size_t r, std::size_t c) const {
    if (r >= row_size_ || c >= col_size_) {
      std::cerr << "Out of Bound" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  std::size_t row_size_ = 0;
  std::size_t col_size_ = 0;

  Type **matrix_ = nullptr;
};

#endif  // MATRIX_LIB_INCLUDE_MATRIX_H_