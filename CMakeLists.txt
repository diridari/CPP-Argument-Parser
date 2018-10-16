cmake_minimum_required(VERSION 3.10)
project(CPP_Argument_Parser)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

set(tests_src tests/argvParserTest.cpp tests/configReaderTest.cpp)
set(src src/argvParser.cpp src/argvParser.h src/configFileReader.cpp src/configFileReader.h)


add_executable(CPP_Argument_Parser main.cpp ${src})
add_executable(CPP_Argument_Parser_Test  ${tests_src} ${src})

target_link_libraries(CPP_Argument_Parser_Test  ${GTEST_BOTH_LIBRARIES} pthread)