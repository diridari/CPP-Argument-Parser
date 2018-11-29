#
#  CONTENT
#    simple Makefile to build all source for GdbManipulator and GdbManipulatorTest
#
#  AUTHOR
#    Sebastian Balz
#
#  DATE
#    2018-03-13
#
#  LANGUAGE
#    make
#


all::
	make -f Makefile.test
	make -f Makefile.app

test :
	make -f Makefile.test
	./CPP-Argument-Parser-Test

clean ::
	make  -f  Makefile.test clean
	make  -f  Makefile.app clean

run ::
	make -f Makefile.app
	./CPP-Argument-Parser

