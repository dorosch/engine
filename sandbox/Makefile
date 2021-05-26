LIB = $(shell dirname $(shell pwd))/engine

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS)
INCLUDE = -I $(LIB)/include -L $(LIB)/bin
LDFLAGS = -lengine

TARGET_DIR = bin
TARGET = $(TARGET_DIR)/application
SOURCES = $(shell find ./src/ -name *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


$(TARGET): $(OBJECTS)
	@make --dir $(LIB) -f $(LIB)/Makefile
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(TARGET)
	@LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(LIB)/bin ./$(TARGET)

clean:
	@rm -f $(OBJECTS)
	@rm -rf $(TARGET_DIR)
