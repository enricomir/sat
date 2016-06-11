CXX	:= g++
CXXFLAGS := -O3 -std=c++11 # -g -pg
INCLUDES := -I./inc/
LIBS := 

OBJ := obj/main.o
BIN := knn_dist.app

$(phony all): knn

$(phony knn): $(OBJ)
	$(CXX) $(inputs) $(CXXFLAGS) -o ./bin/$(BIN) $(LIBS)

obj/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(input) -o $(output)

$(phony clean):
	rm -rf bin/*
	rm -rf obj/*
