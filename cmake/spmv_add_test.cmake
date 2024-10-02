macro(spmv_add_test FILENAME)
  # Strip the path and extension from the filename to get the test name
  set(TESTNAME ${FILENAME})
  get_filename_component(TESTNAME ${TESTNAME} NAME_WE)
  get_filename_component(TESTNAME ${TESTNAME} NAME_WLE)
  # Prepend "test_" to the test name
  set(TESTNAME "test_${TESTNAME}")

  add_executable(${TESTNAME} ${FILENAME})

  target_link_libraries(${TESTNAME} PRIVATE spmv) 

  add_test(${TESTNAME} ${TESTNAME})

  set_target_properties(${TESTNAME} PROPERTIES CXX_STANDARD ${SPMV_CXX_STANDARD})
endmacro()
