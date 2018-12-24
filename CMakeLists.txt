cmake_minimum_required(VERSION 3.10)
project(CPP_Argument_Parser)

include_directories(include)
set(CMAKE_CXX_STANDARD 11)

set(src src/argvParser.cpp include/argvParser.h src/configFileReader.cpp src/configFileReader.h)
add_executable(argParserExp ${src} exampleMain.cpp)

find_package(GTest)
if( GTest_FOUND)
    message(" -- gtest found at " ${GTEST_INCLUDE_DIRS})
    include_directories(${GTEST_INCLUDE_DIRS})
    set(tests_src tests/argvParserTest.cpp tests/configReaderTest.cpp tests/exampleTest.cpp tests/definedNumberOfArgs.cpp)
    add_executable(argParserExp_Test  ${tests_src} ${src})
    target_link_libraries(argParserExp_Test  ${GTEST_BOTH_LIBRARIES} pthread)
else()
    message( WARNING "to enable self test install gtest")
endif()

ADD_LIBRARY( CPPArgvParsLib STATIC ${src})
#ADD_LIBRARY(CPPArgvParsTestLib STATIC ${tests_src} ${src})
