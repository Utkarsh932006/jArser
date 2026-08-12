
CXX = g++
CXXFLAGS  = -std=c++17 -march=native -flto -O3 -Wall -Wextra -Werror -Wformat-security -Wconversion -fstack-protector-strong
TARGET = jsonParser

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)