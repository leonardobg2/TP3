CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude -g

SRC = src/main.cpp
OBJ = obj/main.o
EXEC = bin/tp3.out

all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)

obj/main.o: src/main.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o obj/main.o

run: $(EXEC)
	./$(EXEC) $(INPUT)

r: $(EXEC)
	./$(EXEC) input_8.txt

clean:
	rm -rf obj bin
