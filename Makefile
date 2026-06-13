CXX = g++ 
CXX_FLAGS = -std=c++17 -Wall -Wextra -Wshadow
TARGET = snake

$(TARGET): main.cpp 
	$(CXX) $(CXX_FLAGS) main.cpp -o $(TARGET).exe

run: $(TARGET)
	./$(TARGET).exe

clean:
	rm -f $(TARGET).exe