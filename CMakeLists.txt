cmake_minimum_required(VERSION 3.10)
project(CPP_Argument_Parser)

include_directories(include)
set(CMAKE_CXX_STANDARD 11)

set(src src/argvParser.cpp include/argvParser.h src/configFileReader.cpp src/configFileReader.h src/argParserAdvancedConfiguration.h src/argParserAdvancedConfiguration.cpp)
add_executable(extendedExmple ${src} exampleMainExtended.cpp)
add_executable(simpleExample ${src} exampleMainSimple.cpp)

find_package(GTest)
if( GTest_FOUND)
    message(" -- gtest found at " ${GTEST_INCLUDE_DIRS})
    include_directories(${GTEST_INCLUDE_DIRS})
    set(tests_src tests/argvParserTest.cpp tests/configReaderTest.cpp tests/exampleTest.cpp tests/definedNumberOfArgs.cpp tests/advancedConfigurationTest.cpp tests/lambdaTest.cpp)
    add_executable(argParser_Test  ${tests_src} ${src})
    target_link_libraries(argParser_Test  ${GTEST_BOTH_LIBRARIES} pthread)
    enable_testing()
    gtest_discover_tests(argParser_Test tests
            )
else()
    message( WARNING "to enable self test install gtest")

endif()

ADD_LIBRARY( CPPArgvParsLib STATIC ${src})
#ADD_LIBRARY(CPPArgvParsTestLib STATIC ${tests_src} ${src})
