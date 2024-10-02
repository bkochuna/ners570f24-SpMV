#pragma once

// In order for "assert" to work, NDEBUG cannot be defined.
// However, NDEBUG is defined for CMake Release and RelWithDebInfo builds.
// In order to safely undef NDEBUG, allowing assert to work, all of the headers we
// want to use must be included before we undef NDEBUG.
// Therefore, we check that _LIBSPMV_, a macro defined in the top-level header file, is
// defined. Otherwise, we throw an error.

#ifndef _LIBSPMV_
#  error("unit_test_framework.hpp must be included AFTER SpMV.hpp!")
#endif

#include <cstdio>  // printf
#include <cstdlib> // std::abs, exit
#include <iostream> // std::cout, std::endl

#undef NDEBUG
#include <cassert>

// Overview:
// 1. Use TEST_CASE(test_name) to define a test case containing one or more 
//    ASSERT statements.
// 2. Use TEST_SUITE(suite_name) to define a test suite containing one or more
//    TEST(test_name), which will run the test named test_name.
// 3. Use RUN_SUITE(suite_name) to run a test suite in the main function.
//
// Similar to the C++ testing framework Catch2, but much simpler.

#define ASSERT(cond) assert(cond)

#define ASSERT_NEAR(a, b, eps)                                                           \
  {                                                                                      \
    auto const a_eval = (a);                                                             \
    auto const b_eval = (b);                                                             \
    auto const diff = std::abs(a_eval - b_eval);                                         \
    assert(diff <= (eps));                                                               \
  }

#define TEST_CASE(name) void name()

#define TEST_SUITE(name) void name()

#define TEST(name)                                                                       \
  printf("Running test case '%s'\n", #name);                                             \
  name();                                                                                \
  printf("Test case '%s' passed\n", #name);

#define RUN_SUITE(suite)                                                                 \
  printf("Running test suite '%s'\n", #suite);                                           \
  suite();                                                                               \
  printf("Test suite '%s' passed\n", #suite);
