# Define the symbols we might want to change:
CXX	:= g++
CXXFLAGS := -g -O0 -std=c++11
INCLUDES := -I./inc/

$(phony all): obj/*.o
	$(CXX) $(inputs) -o ./bin/sat.app

obj/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(input) -o $(output)

$(phony setup):
	mkdir bin
	mkdir obj

$(phony clean):
	rm -rf bin/*
	rm -rf obj/*
