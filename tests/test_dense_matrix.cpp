#include "SpMV.hpp"
#include <vector>
#include <stdexcept>
#include "unit_test_framework.hpp"

// Test default construction
TEST_CASE(test_den_default_constructor) {
    DenseMatrix matrix;
    ASSERT(matrix.getNRows() == 0);
    ASSERT(matrix.getNCols() == 0);
    ASSERT(matrix.getData().empty());
}

// Test parameterized construction
TEST_CASE(test_den_parameterized_constructor) {
    // Test normal case
    DenseMatrix matrix(2, 3);
    ASSERT(matrix.getNRows() == 2);
    ASSERT(matrix.getNCols() == 3);
    ASSERT(matrix.getData().size() == 2);
    ASSERT(matrix.getData()[0].size() == 3);

    // Test edge cases
    DenseMatrix matrix_zero(0, 0);
    ASSERT(matrix_zero.getNRows() == 0);
    ASSERT(matrix_zero.getNCols() == 0);

    DenseMatrix matrix_single(1, 1);
    ASSERT(matrix_single.getNRows() == 1);
    ASSERT(matrix_single.getNCols() == 1);

    // Test invalid dimensions - should throw
    bool caught_exception = false;
    try {
        DenseMatrix invalid(-1, 5);
    } catch (const std::invalid_argument&) {
        caught_exception = true;
    }
    ASSERT(caught_exception);
}

// Test copy construction
TEST_CASE(test_den_copy_constructor) {
    DenseMatrix original(2, 2);
    original.getData()[0][0] = 1.0;
    original.getData()[0][1] = 2.0;
    original.getData()[1][0] = 3.0;
    original.getData()[1][1] = 4.0;

    DenseMatrix copy(original);
    ASSERT(copy.getNRows() == original.getNRows());
    ASSERT(copy.getNCols() == original.getNCols());
    
    // Verify deep copy
    ASSERT(copy.getData()[0][0] == original.getData()[0][0]);
    copy.getData()[0][0] = 5.0;
    ASSERT(copy.getData()[0][0] != original.getData()[0][0]);
}

// Test move construction
TEST_CASE(test_den_move_constructor) {
    DenseMatrix original(2, 2);
    original.getData()[0][0] = 1.0;
    
    DenseMatrix moved(std::move(original));
    ASSERT(moved.getNRows() == 2);
    ASSERT(moved.getNCols() == 2);
    ASSERT(moved.getData()[0][0] == 1.0);
    
    // Original should be in valid but empty state
    ASSERT(original.getNRows() == 0);
    ASSERT(original.getNCols() == 0);
    ASSERT(original.getData().empty());
}

// Test destruction (memory leaks)
TEST_CASE(test_den_destructor) {
    // Create and destroy matrix multiple times
    for (int i = 0; i < 1000; ++i) {
        DenseMatrix matrix(100, 100);
        // Matrix will be automatically destroyed at end of each iteration
    }
    // If we reach here without memory issues, destruction worked correctly
}

// Group all tests in a suite
TEST_SUITE(dense_matrix_construction_suite) {
    TEST(test_den_default_constructor);
    TEST(test_den_parameterized_constructor);
    TEST(test_den_copy_constructor);
    TEST(test_den_move_constructor);
    TEST(test_den_destructor);
}

int main() {
    RUN_SUITE(dense_matrix_construction_suite);
    return 0;
}
