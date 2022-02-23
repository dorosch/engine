LIB = $(shell dirname $(shell pwd))/engine

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS)
INCLUDE = -I $(LIB)/include -L $(LIB)/target/lib
LDFLAGS = -lengine -lfmt

TARGET_DIR = bin
TARGET = $(TARGET_DIR)/application
SOURCES = $(shell find ./src/ -name *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


$(TARGET): $(OBJECTS)
	$(MAKE) -C $(LIB)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(TARGET)
	@LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(LIB)/target/lib ./$(TARGET)

run: $(TARGET)
	@LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(LIB)/target/lib ./$(TARGET)

clean:
	@rm -f $(OBJECTS)
	@rm -rf $(TARGET_DIR)
