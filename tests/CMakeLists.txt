include(${PROJECT_SOURCE_DIR}/cmake/spmv_add_test.cmake)

spmv_add_test(./example_unit_test.cpp)
spmv_add_test(./COO_constructor_destructor_unit_test.cpp)
