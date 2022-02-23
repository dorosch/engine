CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS)
INCLUDE = -I include -I ../../include

TARGET_DIR = lib
TARGET = $(TARGET_DIR)/libeditor.a
SOURCES = editor.cpp
OBJECTS = $(SOURCES:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	ar rs $(TARGET) $(OBJECTS)

$(OBJECTS): $(SOURCES)

clean:
	@rm -rf $(TARGET_DIR) $(OBJECTS)
