cmake_minimum_required(VERSION 3.20)

project(life)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS False)
set(SFML_USE_STATIC_STD_LIBS True)

add_subdirectory(libs/SFML)

file(GLOB SOURCE_FILES src/*.cpp src/*.h)

add_executable(life ${SOURCE_FILES})


target_link_libraries(life sfml-graphics sfml-window sfml-audio sfml-network sfml-system)