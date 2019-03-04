# Cmake version
cmake_minimum_required(VERSION 2.8.9)

# Project name
project(twitter)

# Bring the headers into the project
include_directories(src/include)

# file(GLOB...) allows for wildcard additions:
file(GLOB_RECURSE SOURCES "src/*.c")

# Link .o with the main executable file
add_executable(main ${SOURCES})

