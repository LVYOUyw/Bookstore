CXX = g++
CXXFLAGS = -std=c++17 -O3

code: Bookstore.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)
