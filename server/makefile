rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

CC := g++
CFLAGS := -Wall
LIBS := -lm

SRC := src
OBJ := out/obj
INC := include
TARGET := out/main.out

SOURCES := $(call rwildcard,$(SRC),*.cpp)
INCLUDES := $(call rwildcard,$(INC),*.h)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

all: clean mkobj $(TARGET)
	rm -rf $(OBJ)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LIBS) -o $@

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDES)
	mkdir -p `dirname $@`
	$(CC) -I$(SRC) -I$(INC) -c $< -o $@

mkobj:
	mkdir -p $(OBJ)

clean:
	rm -rf $(OBJ)
	rm -f $(TARGET)

.PHONY: clean mkobj all