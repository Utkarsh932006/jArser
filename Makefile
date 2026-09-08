
CXX ?= g++
CXXFLAGS ?= -std=c++23 -O3 -Wall -Wextra -Werror -Wno-maybe-uninitialized -Wformat-security -Wconversion -fstack-protector-strong
TARGET = jsonParser

.PHONY: all check clean

all: $(TARGET)

$(TARGET): $(TARGET).cpp $(TARGET).hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp

check: $(TARGET)
	./$(TARGET) jsonExample.json

clean:
	$(RM) $(TARGET)
