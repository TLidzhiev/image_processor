cmake_minimum_required(VERSION 3.23)
project(image_processor)

set(CMAKE_CXX_STANDARD 23)

add_executable(image_processor main.cpp
        Parser.h
        Parser.cpp
        Image.h
        Image.cpp
        Filter.h
        Filter.cpp
        )

