CXX = g++
CXXFLAGS = -fPIC -std=c++17 -O2 -Wall -pedantic -I include $(LDFLAGS)
LDFLAGS =

TARGET_DIR = bin
TARGET = $(TARGET_DIR)/engine.so
SOURCES = $(shell find src/ -name *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) -shared $(OBJECTS) $(CXXFLAGS) -o $(TARGET)

clean:
	@rm -rf $(TARGET_DIR)
	@rm -f $(shell find src/ -name *.o)
