CXX = g++
CXXFLAGS = -fPIC -std=c++20 -O2 -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS)
INCLUDE = -I include
LDFLAGS = 

TARGET_DIR = bin
TARGET = $(TARGET_DIR)/libengine.so
SOURCES = $(shell find ./src/ -name *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) -shared $(OBJECTS) $(CXXFLAGS) -o $(TARGET)

clean:
	@rm -rf $(TARGET_DIR)
	@rm -f $(OBJECTS)
