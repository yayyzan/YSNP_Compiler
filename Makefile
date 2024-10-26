TARGET = bin/c_compiler
CXX = g++

CXXFLAGS += -std=c++17 -I include -I src

HEADER_FILES := $(wildcard include/*.hpp)
SOURCE_FILES := $(wildcard src/*.cpp)
OBJECT_FILES = $(SOURCE_FILES:.cpp=.o)

all : $(TARGET)

$(TARGET) : src/lexer.yy.o src/parser.tab.o $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

src/%.o: src/%.cpp $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/parser.tab.cpp src/parser.tab.hpp: src/parser.y
	bison -d src/parser.y -o src/parser.tab.cpp
	mkdir -p bin;

src/lexer.yy.cpp : src/lexer.flex src/parser.tab.hpp
	flex -o src/lexer.yy.cpp src/lexer.flex

src/parser.tab.o: src/parser.tab.cpp src/parser.tab.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/lexer.yy.o: src/lexer.yy.cpp src/parser.tab.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean :
	rm -rf bin/*
	find src/ -type f -name '*.o' -delete
	rm -f src/parser.tab.cpp src/parser.tab.hpp src/lexer.yy.cpp
