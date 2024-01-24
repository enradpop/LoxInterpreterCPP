# CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(LoxCPP)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your source files (both .cpp and .h) to the project
file(GLOB SOURCES "*.cpp" "*.h")

# Add the Python script as a custom target to generate .h file
add_custom_target(GenerateAst COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tool/GenerateAst.py ${CMAKE_CURRENT_SOURCE_DIR})

# Add the main executable
add_executable(Lox ${SOURCES})

# Depend on the custom target, so the script is run before building the executable
add_dependencies(Lox GenerateAst)