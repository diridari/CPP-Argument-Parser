#
#  CONTENT
#    simple Makefile.app to build all source for GdbManipulator
#
#  AUTHOR
#   Sebastian Balz
##		based on Norbert Stoeffler
#
#  DATE
#    29.11.2018
#
#  LANGUAGE
#    make
#

APP =		CPP-Argument-Parser
TEST =		CPP-Argument-Parser-TEST
APP_SRCS =	../exampleMain.cpp $(shell find \
			../src \
			-name "*.cpp")

APP_INCS =
TEST_INCS =
APP_SLIB =

include		linux.mk