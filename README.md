# Conway's Game of Life
Implementation of the famous Game of Life devised by the John Horton Conway
## Prerequisites
- Git 2.13 or newer (for submodules support)
- CMake 3.20 or newer
- Compiler supporting C++17
## How to run
Clone the repository and submodules from github
```
git clone --recurse-submodules git@github.com:jjkasprzak/life.git
```
Build the project using CMake
```
cd life/build
cmake ..
cmake --build .
```
Locate and run the executable (location of executable can vary based on generator used by CMake)


