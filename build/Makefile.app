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

APP =		argParserExp
TEST =		argParserExp-TEST
APP_SRCS =	../exampleMain.cpp $(shell find \
			../src \
			-name "*.cpp")

APP_INCS = ../include
TEST_INCS =
APP_SLIB =

include		linux.mk
