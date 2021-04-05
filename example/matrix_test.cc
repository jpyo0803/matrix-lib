#include "matrix/matrix.h"
#include "matrix/matrix_simd.h"

#include <benchmark/benchmark.h>

#include <iostream>
#include <chrono>

using namespace std::chrono;

static void BM_NaiveWay(benchmark::State& state) {
  // Perform setup here
  Matrix<float, 70, 30> mat1;
  Matrix<float, 30, 40> mat2;
  Matrix<float, 40, 60> mat3;
  Matrix<float, 60, 100> mat4;

  mat1.SetOnes();
  mat2.SetOnes();
  mat3.SetOnes();
  mat4.SetOnes();

  for (auto _ : state) {
    // This code gets timed
    auto res = mat1 * mat2 * mat3 * mat4;
    // benchmark::DoNotOptimize(res);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_NaiveWay);

static void BM_SimdWay(benchmark::State& state) {
  // Perform setup here
  MatrixSimd<float, 70, 30> mats1;
  MatrixSimd<float, 30, 40> mats2;
  MatrixSimd<float, 40, 60> mats3;
  MatrixSimd<float, 60, 100> mats4;

  mats1.SetOnes();
  mats2.SetOnes();
  mats3.SetOnes();
  mats4.SetOnes();
  
  for (auto _ : state) {
    // This code gets timed
    auto res = mats1 * mats2 * mats3 * mats4;
    // benchmark::DoNotOptimize(res);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SimdWay);

// Run the benchmark
BENCHMARK_MAIN();

// int main() {
//   Matrix<float, 70, 30> mat1;
//   Matrix<float, 30, 40> mat2;
//   Matrix<float, 40, 60> mat3;
//   Matrix<float, 60, 100> mat4;

//   MatrixSimd<float, 70, 30> mats1;
//   MatrixSimd<float, 30, 40> mats2;
//   MatrixSimd<float, 40, 60> mats3;
//   MatrixSimd<float, 60, 100> mats4;

//   mat1.SetOnes();
//   mat2.SetOnes();
//   mat3.SetOnes();
//   mat4.SetOnes();

//   mats1.SetOnes();
//   mats2.SetOnes();
//   mats3.SetOnes();
//   mats4.SetOnes();

//   auto t1s = steady_clock::now();
//   auto res = mat1 * mat2 * mat3 * mat4;
//   auto t1e = steady_clock::now();
//   std::cout << (t1e - t1s).count() << std::endl;
//   res.Print();

//   auto t2s = steady_clock::now();
//   auto ress = mats1 * mats2 * mats3 * mats4;
//   auto t2e = steady_clock::now();
//   std::cout << (t2e - t2s).count() << std::endl;
//   ress.Print();
//   return 0;
// }