add_library(${LIB_NAME} "SampleStream.cpp")
add_executable(${EXEC_NAME} main.cpp)
target_link_libraries(${EXEC_NAME} ${LIB_NAME})