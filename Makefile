CXX = g++
CXX_FLAGS = -std=c++17 -Wall -Wextra -Wshadow -Iinclude
TARGET = snake
SOURCES = src/main.cpp src/game.cpp src/terminal.cpp
HEADERS = include/types.hpp include/game.hpp include/terminal.hpp

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXX_FLAGS) $(SOURCES) -lncurses -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)