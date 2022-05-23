cmake_minimum_required(VERSION 3.20)

project(life)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS False)

add_subdirectory(libs/SFML)

file(GLOB SOURCE_FILES src/*.cpp src/*.h)

add_executable(life ${SOURCE_FILES})

add_custom_command(TARGET life PRE_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory 
                    ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:life>)

target_link_libraries(life sfml-graphics sfml-window sfml-audio sfml-network sfml-system)