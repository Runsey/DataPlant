CXX      := g++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -Wpedantic
LDFLAGS  :=

TARGET   := mdp_ingest
SRC      := src/ingest/main.cpp src/ingest/udp_reader.cpp
OBJ      := $(SRC:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

