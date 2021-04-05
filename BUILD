load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
  name = "matrix",
  hdrs = ["include/matrix/matrix.h"],
  includes = ["include"],
  copts = [
    # "-march=native",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "matrix_simd",
  hdrs = ["include/matrix/matrix_simd.h"],
  includes = ["include"],
  # cxxopt = [
  #   # "-mavx2",
  # ],
  visibility = ["//visibility:public"],
)