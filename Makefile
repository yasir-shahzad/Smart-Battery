.PHONY: BmsI2C

BmsI2C: examples/ReadParameters.cpp
	g++ -std=c++17 examples/ReadParameters.cpp src/BmsI2C.cpp -li2c -o bin/BmsI2C


