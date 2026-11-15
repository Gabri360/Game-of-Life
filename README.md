# Game Of Life
## Contents
- [Introduction](#introduction)
- [Usage and Dependency](#usage-and-dependency)
- [Comand](#comand)
## Introduction
This project is a personal remake of the **John Conway's Game Of Life**, written entirely in C (and CMake).


## Usage and Dependency
To install the program, a C compiler and `CMake` are required. Once you have downloaded and opened the folder on a terminal run:
```bash
cmake -S . -B build
cmake --build build
```
This will create the executable in the `build/` folder and can be run with `./game_of_life` on linux.

## Comand
| Key | |
|---------|-------------|
| `ENTER` | Start and stop the simulation |
| `Q` | Quit |
| `R` | Restart |
| `I` | Invert grid |
| `UP` | accelerate simulation |
| `DOWN` | decelerate simulation |
