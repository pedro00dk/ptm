CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix bin/,$(notdir $(CPP_FILES:.cpp=.o)))
CC_FLAGS := -std=c++11 -o3
RM := rm -f

bin:
	g++ $(CC_FLAGS) src/ptm.cpp -o bin/ptm.o

clean: ; $(RM) $(OBJ_FILES)

.PHONY: bin clean

