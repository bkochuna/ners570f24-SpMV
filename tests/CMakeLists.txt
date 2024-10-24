include(${PROJECT_SOURCE_DIR}/cmake/spmv_add_test.cmake)

spmv_add_test(./example_unit_test.cpp)
#spmv_add_test(./ell_storage_test.cpp)
spmv_add_test(./ELL_view_test.cpp)
spmv_add_test(./ELL_matvec_test.cpp)