CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix bin/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -o3
CC_FLAGS := -o3

ptm.exe: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

bin/%.o: src/%.cpp
	g++ $(CC_FLAGS) -c -o $@ $<