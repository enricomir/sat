# Define the symbols we might want to change:
CXX	:= g++
CXXFLAGS := -g -O0 -std=c++11
INCLUDES := -I./inc/ -I/usr/include/paradiseo -I/usr/include/paradiseo/eo
LIBS := /usr/lib64/libcma.a /usr/lib64/libeo.a /usr/lib64/libeoutils.a /usr/lib64/libes.a /usr/lib64/libga.a /usr/lib64/libmoeo.a

$(phony all): obj/*.o
	$(CXX) $(inputs) -o ./bin/sat.app $(LIBS)

obj/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(input) -o $(output)

$(phony setup):
	mkdir bin
	mkdir obj

$(phony clean):
	rm -rf bin/*
	rm -rf obj/*
