macro(spmv_add_exe FILENAME)
  # Strip the path and extension from the filename to get the exe name
  set(EXEC ${FILENAME})
  get_filename_component(EXEC ${EXEC} NAME_WE)
  get_filename_component(EXEC ${EXEC} NAME_WLE)

  add_executable(${EXEC} ${FILENAME})

  target_link_libraries(${EXEC} spmv) 

  set_target_properties(${EXEC} PROPERTIES CXX_STANDARD ${SPMV_CXX_STANDARD})
endmacro()
