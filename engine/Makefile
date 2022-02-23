CXX = g++
CXXFLAGS = -fPIC -std=c++17 -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS)
INCLUDE = -I include
LDFLAGS = -lglfw -lGLEW -lfmt

TARGET_DIR = target/lib
TARGET = $(TARGET_DIR)/libengine.so
TARGET_TEST = $(TARGET_DIR)/test
SOURCES = $(shell find ./src/ -name *.cpp -not -path "./src/editor/*")
SOURCES_TEST = $(shell find ./test/ -name *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS_TEST = $(SOURCES_TEST:.cpp=.o)
LIBRARIES = src/editor/lib/libeditor.a


all: editor $(TARGET)

editor: $(OBJECTS)
	$(MAKE) -C src/editor

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) -shared $(OBJECTS) $(LIBRARIES) $(CXXFLAGS) -o $(TARGET)

test: $(TARGET) $(OBJECTS_TEST)
	$(CXX) $(OBJECTS_TEST) $(CXXFLAGS) -o $(TARGET_TEST) -L $(TARGET_DIR) -lengine
	@LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(TARGET_DIR) ./$(TARGET_TEST)

clean:
	$(MAKE) -C src/editor clean
	@rm -rf $(TARGET_DIR)
	@rm -f $(OBJECTS) $(OBJECTS_TEST)
