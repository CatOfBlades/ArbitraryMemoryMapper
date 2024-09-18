cmake_minimum_required(VERSION 3.10)

# Find all .cpp files in the source directory
file(GLOB SOURCES "${SOURCE_DIR}/*.c")

add_library(LuaLib OBJECT ${SOURCES})