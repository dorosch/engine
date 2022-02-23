IMGUI_DIR = imgui-1.87
TARGET_DIR = lib
TARGET = $(TARGET_DIR)/libeditor.a
SOURCES = editor.cpp $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp 
SOURCES += $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJECTS = $(SOURCES:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS) -lGL `pkg-config --static --libs glfw3` `pkg-config --cflags glfw3` -fPIC
INCLUDE = -I include -I ../../include -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends


all: dependencies $(TARGET)

dependencies:
	wget -nc https://github.com/ocornut/imgui/archive/refs/tags/v1.87.tar.gz -O - | tar -xz

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	ar rs $(TARGET) $(OBJECTS)

$(OBJECTS): $(SOURCES)

clean:
	@rm -rf $(TARGET_DIR) $(OBJECTS)
