

CXX = g++
CXXFLAGS = -O3 -pthread


.DEFAULT_GOAL = main

main: client.cpp server.cpp hash.h
	$(CXX) $(CXXFLAGS) client.cpp -o client
	$(CXX) $(CXXFLAGS) server.cpp -o server