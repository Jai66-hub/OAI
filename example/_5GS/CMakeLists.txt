cmake_minimum_required(VERSION 2.6) 
find_package(GTest REQUIRED) 
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/../../include)
link_directories(${CMAKE_SOURCE_DIR}/../../o)
add_executable(runTests identity_response_code_decode.cpp) 
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread nascodec)
