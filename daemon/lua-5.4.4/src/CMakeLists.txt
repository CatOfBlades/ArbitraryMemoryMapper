cmake_minimum_required(VERSION 3.10)

# Specify the source directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")  # Adjust as needed

# Find all .c files in the source directory
file(GLOB SOURCES "${SOURCE_DIR}/*.c")

# Create an object library from the source files
add_library(LuaLib OBJECT ${SOURCES})
