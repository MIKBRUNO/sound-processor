cmake_minimum_required (VERSION 3.14)
project (SoundProcessor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(EXEC_NAME sound_processor)
set(LIB_NAME splib)
# set(TEST_NAME tests)

add_subdirectory (src) 

# enable_testing()

# add_subdirectory ("test")

