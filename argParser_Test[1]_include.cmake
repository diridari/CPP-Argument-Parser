if(EXISTS "/home/basto/git/CPP-Argument-Parser/argParser_Test[1]_tests.cmake")
  include("/home/basto/git/CPP-Argument-Parser/argParser_Test[1]_tests.cmake")
else()
  add_test(argParser_Test_NOT_BUILT argParser_Test_NOT_BUILT)
endif()
