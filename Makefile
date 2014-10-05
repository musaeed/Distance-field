CC = g++

SRC = src
FLAGS = -lglut -lGLU -lGL -lassimp -std=c++11
OBJ = build
BIN = bin
TARGET = exec
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst %.cpp,$(OBJ)/%.o,$(notdir $(wildcard $(SRC)/*.cpp)))

all: $(OBJECTS)
	mkdir -p $(BIN)
	$(CC) $(OBJECTS) -o $(BIN)/$(TARGET) $(FLAGS)

$(OBJ)/%.o : %.cpp
	mkdir -p $(OBJ)
	$(CC) -c $(FLAGS) -o $@ $<

vpath %.cpp $(SRC)

run: all
	./$(BIN)/$(TARGET)

clean:
	@rm -rf $(BIN)
	@rm -rf $(OBJ)