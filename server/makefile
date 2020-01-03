rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

CC := g++
CFLAGS := -Wall -std=c++17 -g
LIBS := -lm -lpthread -lcurl

SRC := src
OBJ := out/obj
INC := include
TARGET := out/main.out

SOURCES := $(call rwildcard,$(SRC),*.cpp)
INCLUDES := $(call rwildcard,$(INC),*.h)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LIBS) -o $@

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDES)
	mkdir -p `dirname $@`
	$(CC) ${CFLAGS} -I$(SRC) -I$(INC) -c $< -o $@

mkobjdir:
	mkdir -p $(OBJ)

clean:
	rm -rf $(OBJ)
	rm -f $(TARGET)
