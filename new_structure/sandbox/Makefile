TARGET = app
ENGINE = $(shell dirname $(shell pwd))/engine

CXX = g++
INCLUDE = -I $(ENGINE)/include
LDFLAGS = -L $(ENGINE) -lradian
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic $(INCLUDE) $(LDFLAGS)
SOURCES = $(shell find ./ -name *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(ENGINE) ./$(TARGET)

$(TARGET): engine $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(TARGET)

engine:
	$(MAKE) -C $(ENGINE)

clean:
	@rm -rf $(TARGET)
	@rm -f $(OBJECTS)
