# Makefile para Coloração Defeituosa

CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall
TARGET = coloracao
SRC = main.cpp Grafo.cpp ColoracaoDefeituosa.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET) resultados.csv *.txt

.PHONY: all clean
