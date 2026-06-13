CXX = g++ 
CXX_FLAGS = -std=c++17 -Wall -Wextra -Wshadow
TARGET = snake

$(TARGET): main.cpp 
	$(CXX) $(CXX_FLAGS) main.cpp -lncurses -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)