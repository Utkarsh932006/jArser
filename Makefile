
CXX ?= g++
CXXFLAGS ?= -std=c++17 -O3 -Wall -Wextra -Werror -Wformat-security -Wconversion -fstack-protector-strong
TARGET = jsonParser

.PHONY: all check clean

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp

check: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)
