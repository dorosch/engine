TARGET = libradian.so

CXX = g++
INCLUDE = -I ./include
LDFLAGS =
CXXFLAGS = -std=c++17 -shared -fPIC -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS)
SOURCES = $(shell find ./src/ -name *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(TARGET)

clean:
	@rm -rf $(TARGET)
	@rm -f $(OBJECTS)
