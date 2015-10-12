# Define the symbols we might want to change:
CXX	:= g++
CXXFLAGS := -g -O2 -std=c++11
INCLUDES := -I./inc/ -I/usr/include/paradiseo -I/usr/include/paradiseo/eo
LIBS := /usr/lib64/libcma.a /usr/lib64/libeo.a /usr/lib64/libeoutils.a /usr/lib64/libes.a /usr/lib64/libga.a /usr/lib64/libmoeo.a

OPT_OBJ := obj/SatProblem.o obj/bitga.o obj/BnB.o
OPT_BIN := ga.app

$(phony all): opt

$(phony opt): $(OPT_OBJ)
	$(CXX) $(inputs) -o ./bin/$(OPT_BIN) $(LIBS)

obj/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(input) -o $(output)

$(phony setup):
	mkdir bin
	mkdir obj

$(phony clean):
	rm -rf bin/*
	rm -rf obj/*
