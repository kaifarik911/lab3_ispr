.PHONY: all clean test format

CXX=g++
OFLAGS=-O2 -Wall -DNDEBUG
LDFLAGS=-lUnitTest++ 

SOURCES=modAlphaCipher.cpp modAlphaCipher-test.cpp
HEADERS=modAlphaCipher.h
EXECUTABLE=modAlphaCipher-test


$(EXECUTABLE): $(SOURCES)
	$(CXX)  $(SOURCES)  -o $(EXECUTABLE) $(OFLAGS) $(LDFLAGS)

test: $(EXECUTABLE)
	./$(EXECUTABLE) 

format:
	astyle $(SOURCES) $(HEADERS)
clean:
	rm -f $(EXECUTABLE) *.orig
