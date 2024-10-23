#include "test_CSR_storage.hpp"
#include "unit_test_framework.hpp"

int main() {
    // Run the test suite for SparseMatrix_CSR construction and destruction
     RUN_SUITE(test_CSR_storage::defaultConstructor);
    RUN_SUITE(test_CSR_storage::parameterizedConstructor_Valid);
    RUN_SUITE(test_CSR_storage::parameterizedConstructor_InvalidDimensions);
     RUN_SUITE(test_CSR_storage::parameterizedConstructor_LargeDimensions);
    RUN_SUITE(test_CSR_storage::nonRealElementCheck);
    RUN_SUITE(test_CSR_storage::destructor_NoMemoryLeaks);

    return 0; // Return 0 if all tests pass
}
