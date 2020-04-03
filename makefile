all: trains

trains: trains.cpp
	g++ -o trains trains.cpp -std=c++17
