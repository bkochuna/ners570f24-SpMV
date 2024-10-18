cmake_minimum_required(VERSION 3.20)

project(SpMV VERSION 0.1.0 LANGUAGES CXX)


#===============================================================================
# Basic CMake configuration
#===============================================================================

# Disable in-source builds
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR
    "In-source builds are not allowed."
    " Create a separate directory for build files and delete CMakeCache.txt.")
endif()

# If no build type is specified, default to Debug 
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, defaulting to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the C++ standard
set(SPMV_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD ${SPMV_CXX_STANDARD} CACHE STRING "Default C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require C++ standard")

# Set module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Check that the compiler supports the C++ standard
set(SPMV_MIN_GCC_VERSION 8.0)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${SPMV_MIN_GCC_VERSION})
    message(FATAL_ERROR "GCC version must be at least ${SPMV_MIN_GCC_VERSION}")
  endif()
else()
  message(WARNING 
          "You are using an unsupported compiler! "
          "Please use GCC >= ${SPMV_MIN_GCC_VERSION}")
endif()

#===============================================================================
# Compiler flags
#===============================================================================

# A macro to check if a compiler flag is supported (provided by CMake)
# https://cmake.org/cmake/help/latest/module/CheckCXXCompilerFlag.html
include(CheckCXXCompilerFlag)

# A macro to check if a compiler flag is supported, and if so, add it
# to the C++ compiler flags
macro(spmv_add_cxx_compiler_flag FLAG)
  # SFLAG is the flag without the hyphen, e.g., "-Wall" -> "Wall"
  string(REGEX REPLACE "-" "" SFLAG ${FLAG})
  # Check if the compiler supports the flag
  check_cxx_compiler_flag(${FLAG} COMPILER_SUPPORT_${SFLAG})
  # If the compiler supports the flag, add it to the C++ compiler flags
  if(COMPILER_SUPPORT_${SFLAG})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
  endif()
endmacro()

# Set warning flags
spmv_add_cxx_compiler_flag("-Wall")
spmv_add_cxx_compiler_flag("-Wextra")
spmv_add_cxx_compiler_flag("-Wcast-align")
spmv_add_cxx_compiler_flag("-Wcast-qual")
spmv_add_cxx_compiler_flag("-Wconversion")
spmv_add_cxx_compiler_flag("-Wdouble-promotion")
spmv_add_cxx_compiler_flag("-Wfloat-equal")
spmv_add_cxx_compiler_flag("-Wimplicit-fallthrough")
spmv_add_cxx_compiler_flag("-Wmissing-noreturn")
spmv_add_cxx_compiler_flag("-Wnon-virtual-dtor")
spmv_add_cxx_compiler_flag("-Wshadow")
spmv_add_cxx_compiler_flag("-Wunused")
spmv_add_cxx_compiler_flag("-Wvla")
spmv_add_cxx_compiler_flag("-Wzero-as-null-pointer-constant")
spmv_add_cxx_compiler_flag("-Wunreachable-code")
spmv_add_cxx_compiler_flag("-Wmissing-noreturn")
spmv_add_cxx_compiler_flag("-Wtautological-compare")
spmv_add_cxx_compiler_flag("-Wundef")
spmv_add_cxx_compiler_flag("-Werror")
spmv_add_cxx_compiler_flag("-Wold-style-cast")
spmv_add_cxx_compiler_flag("-pedantic")

#===============================================================================
# Create the library
#===============================================================================

# Source files
set(SPMV_SOURCES
  "src/SparseMatrix.cpp"
  "src/SparseMatrix_COO.cpp"
  "src/SparseMatrix_ELL.cpp"
)

add_library(spmv SHARED ${SPMV_SOURCES})

target_include_directories(spmv
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#===============================================================================
# Other directories
#===============================================================================

# Enable testing
include(CTest)
enable_testing()
add_subdirectory(tests)

# Add examples
add_subdirectory(examples)
