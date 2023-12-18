

rem Compile the C++ file using g++
g++ -o code_gen code_gen.cpp
.\output\code_gen
g++ -o simulation simulation.cpp
.\output\simulation

rem Pause to keep the command window open
pause
