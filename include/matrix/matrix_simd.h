#ifndef MATRIX_LIB_INCLUDE_MATRIX_SIMD_H_
#define MATRIX_LIB_INCLUDE_MATRIX_SIMD_H_

#include <stdlib.h>
#include <immintrin.h>
#include <iostream>
#include <algorithm>

//https://yunmorning.tistory.com/31

// inline void Testt() {
//   __m256 evens = _mm256_set_ps(2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0);
//   __m256 odds = _mm256_set_ps(2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0);

//   __m256 results = _mm256_add_ps(evens, odds);

//   float* fp = (float*)&results;
//   // for (int i = 0; i < 8; i++) {
//   //   std::cout << fp[i] << " ";
//   // }
//   // std::cout << std::endl;
// }

constexpr std::size_t kAvx2Byte = 256 / 8;

template <typename Type, std::size_t RowSize, std::size_t ColSize>
class MatrixSimd {
 public:
  MatrixSimd() : row_size_(RowSize), col_size_(ColSize), type_size_(kAvx2Byte / sizeof(Type)) {
    aligned_row_size_ = row_size_ + (type_size_ - (row_size_ % type_size_));
    aligned_col_size_ = col_size_ + (type_size_ - (col_size_ % type_size_));
    // matrix_ = (Type **)malloc(aligned_row_size_ * sizeof(Type *));
    // for (std::size_t r = 0; r < aligned_row_size_; r++) {
    //   matrix_[r] = (Type *)malloc(aligned_col_size_ * sizeof(Type));
    //   for (std::size_t c = 0; c < aligned_col_size_; c++) {
    //     matrix_[r][c] = 0;
    //   }
    // }
    matrix_ = (Type **)malloc(aligned_row_size_ * sizeof(Type *));
    for (std::size_t r = 0; r < aligned_row_size_; r++) {
      matrix_[r] = (Type *)aligned_alloc(type_size_ * sizeof(Type), sizeof(Type) * aligned_col_size_);
      for (std::size_t c = 0; c < aligned_col_size_; c++) {
        matrix_[r][c] = 0;
      }
    }
  }

  // Copy Constructor
  MatrixSimd(const MatrixSimd &other) : row_size_(other.row_size_), col_size_(other.col_size_), type_size_(kAvx2Byte / sizeof(Type)) {
    // matrix_ = (Type **)malloc(row_size_ * sizeof(Type *));
    // for (std::size_t r = 0; r < row_size_; r++) {
    //   matrix_[r] = (Type *)malloc(col_size_ * sizeof(Type));
    //   for (std::size_t c = 0; c < col_size_; c++) {
    //     matrix_[r][c] = other.matrix_[r][c];
    //   }
    // }

    matrix_ = (Type **)malloc(row_size_ * sizeof(Type *));
    for (std::size_t r = 0; r < row_size_; r++) {
      matrix_[r] = (Type *)aligned_alloc(type_size_ * sizeof(Type), sizeof(Type) * aligned_col_size_);
      for (std::size_t c = 0; c < col_size_; c++) {
        matrix_[r][c] = other.matrix_[r][c];
      }
    }
  }

  // Assignment Operator
  MatrixSimd &operator=(const MatrixSimd &other) = delete;

  // Addition
  MatrixSimd operator+(const MatrixSimd &other) {
    MatrixSimd result;
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        result.matrix_[r][c] = matrix_[r][c] + other.matrix_[r][c];
      }
    }
    return result;
  }

  // Assign Addition
  MatrixSimd &operator+=(const MatrixSimd &other) {
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        matrix_[r][c] += other.matrix_[r][c];
      }
    }
    return *this;
  }

  // Subtraction
  MatrixSimd operator-(const MatrixSimd &other) {
    MatrixSimd result;
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        result.matrix_[r][c] = matrix_[r][c] - other.matrix_[r][c];
      }
    }
    return result;
  }

  // Assign Subtraction
  MatrixSimd &operator-=(const MatrixSimd &other) {
    for (std::size_t r = 0; r < row_size_; r++) {
      for (std::size_t c = 0; c < col_size_; c++) {
        matrix_[r][c] -= other.matrix_[r][c];
      }
    }
    return *this;
  }


  // MatrixSimd Multiplication
  template <std::size_t N>
  MatrixSimd<Type, RowSize, N> operator*(const MatrixSimd<Type, ColSize, N> &other) {
    MatrixSimd<Type, RowSize, N> result;
    for (std::size_t r = 0; r < result.row_size_; r++) {
      for (std::size_t c = 0; c < result.col_size_; c++) {
        for (std::size_t k = 0; k < col_size_; k += type_size_) {
          Type tmp_col[type_size_];
          for (std::size_t i = 0; i < type_size_; i++) {
            tmp_col[i] = matrix_[i][c];
          }
          __m256 row_vec = _mm256_load_ps(&matrix_[r][k]);
          __m256 col_vec = _mm256_load_ps(&tmp_col[0]);

          __m256 res_vec = _mm256_mul_ps(row_vec, col_vec);
          Type* res = (Type*)&res_vec;

          for (std::size_t i = 0; i < type_size_; i++) {
            result.matrix_[r][c] += res[i];
          }
          // Type tmp_row[8];
          // Type tmp_col[8];

          // std::size_t limit = type_size_;
          // if (col_size_ - k < type_size_) limit = col_size_ - k;

          // for (std::size_t i = 0; i < type_size_; i++) {
          //   if (i < limit) {
          //     tmp_row[i] = matrix_[r][i];
          //     tmp_col[i] = other.matrix_[i][c];
          //   } else {
          //     tmp_row[i] = tmp_col[i] = 0;
          //   }
          // }
          // __m256 row_vec = _mm256_load_ps(&tmp_row[0]);
          // __m256 col_vec = _mm256_load_ps(&tmp_col[0]);

          // __m256 res_vec = _mm256_mul_ps(row_vec, col_vec);
          // Type* res = (Type*)&res_vec;

          // for (std::size_t i = 0; i < limit; i++) {
          //   result.matrix_[r][c] += res[i];
          // }
        }
      }
    }
    return result;
  }

  // Deconstructor
  ~MatrixSimd() {
    for (std::size_t r = 0; r < aligned_row_size_; r++) {
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

  std::size_t aligned_row_size_ = 0;
  std::size_t aligned_col_size_ = 0;

  std::size_t type_size_;

  Type **matrix_ = nullptr;
};

#endif  // MATRIX_LIB_INCLUDE_MATRIX_SIMD_H_