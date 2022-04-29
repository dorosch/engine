TARGET_DIR = target
IMGUI_DIR = imgui-1.87
TARGET = $(TARGET_DIR)/main
SOURCES = $(shell find ./src/ -name *.cpp)
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp
SOURCES += $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJECTS = $(SOURCES:.cpp=.o)

CXX = g++
INCLUDE = -I include -I $(IMGUI_DIR) -I $(IMGUI_DIR)/backends
LDFLAGS = -lSOIL -lGL -lGLU -lGLEW -lglfw -lglut -lfmt -lm -ldl
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS)


all: imgui $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(TARGET)

imgui:
# test -f ./$(IMGUI_DIR) || wget -nc https://github.com/ocornut/imgui/archive/refs/tags/v1.87.tar.gz -O - | tar -xz

run: $(TARGET)
	./$(TARGET)

clean:
	@rm -rf $(TARGET_DIR)
	@rm -f $(OBJECTS)
