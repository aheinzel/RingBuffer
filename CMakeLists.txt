cmake_minimum_required(VERSION 3.16)
project(RingBuffer)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  ringbuffer_test
  test/ringbuffer_test.cpp
)

target_include_directories(
  ringbuffer_test PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  ringbuffer_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ringbuffer_test)
