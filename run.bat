@echo off

rem Compile the C++ file using g++
g++ -o code_gen code_gen.cpp
.\code_gen

if %errorlevel% equ 0 (
    rem Run the compiled executable
    g++ -o simulation simulation.cpp
    .\simulation
) else (
    echo code_gen.cpp compilation failed.
)

rem Pause to keep the command window open
