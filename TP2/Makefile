

CXX = g++
CXXFLAGS = -O3 -pthread -std=c++17

BIN = bin/
SRC = src/




.DEFAULT_GOAL = main

main: $(SRC)client.cpp $(SRC)server.cpp $(SRC)hash.h
	$(CXX) $(CXXFLAGS) $(SRC)client.cpp -o $(BIN)client
	$(CXX) $(CXXFLAGS) $(SRC)server.cpp -o $(BIN)server

clean:
	rm -rf bin/*