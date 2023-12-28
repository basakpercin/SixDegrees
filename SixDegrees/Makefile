#
# Makefile for Six Degrees
#
# Project 2 (fall 2020)
#
# Edited by Basak Percin, July - August 2022 
#	Used for the Six Degrees of Collaboration project
# 

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11 
INCLUDES = $(shell echo *.h)

SixDegrees: main.o SixDegrees.o CollabGraph.o Artist.o
	${CXX} -o $@ $^

test: test_SixDegrees
	./test_SixDegrees

test_SixDegrees: test_main.o SixDegrees.o CollabGraph.o Artist.o
	${CXX} -o $@ $^

%.o: %.cpp ${INCLUDES}
	${CXX} ${CXXFLAGS} -c $<

clean:
	rm -rf SixDegrees test_SixDegrees *.o *.dSYM

##############################################
# PLEASE DO NOT FORGET TO ADD ANY ADDITIONAL #
# FILES YOU MADE, SUCH AS TESTING FILES.     #
##############################################