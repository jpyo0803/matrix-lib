load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
  name = "matrix_test",
  srcs = ["matrix_test.cc"],
  deps = [
    "//:matrix",
    "//:matrix_simd",
    "@com_github_google_benchmark//:benchmark",
  ],
)