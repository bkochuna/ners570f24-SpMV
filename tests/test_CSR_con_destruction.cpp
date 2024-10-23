#include "test_CSR_con_destruction.hpp"
#include "unit_test_framework.hpp"

int main() {
    // Run the test suite for SparseMatrix_CSR construction and destruction
     RUN_SUITE(test_CSR_con_destruction::defaultConstructor);
    RUN_SUITE(test_CSR_con_destruction::parameterizedConstructor_Valid);
    RUN_SUITE(test_CSR_con_destruction::parameterizedConstructor_InvalidDimensions);
     RUN_SUITE(test_CSR_con_destruction::parameterizedConstructor_LargeDimensions);
    RUN_SUITE(test_CSR_con_destruction::nonRealElementCheck);
    RUN_SUITE(test_CSR_con_destruction::destructor_NoMemoryLeaks);

    return 0; // Return 0 if all tests pass
}
