#!/bin/bash

# Compile code_gen.cpp to generate simulation.cpp
g++ -o code_gen code_gen.cpp

# Run the code_gen executable to generate simulation.cpp
./code_gen

# Compile simulation.cpp
g++ -o simulation simulation.cpp

# Run the simulation executable
./simulation