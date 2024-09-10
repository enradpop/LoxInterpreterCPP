cmake_minimum_required(VERSION 3.12)
project(LoxCPP)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

# Optionally, set linker flags if needed
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Expr.h
#     COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tool/GenerateAst.py ${CMAKE_CURRENT_SOURCE_DIR}
#     MAIN_DEPENDENCY tool/GenerateAst.py
# )

file(GLOB SOURCES "*.cpp" "*.h")

# Add the main executable
# add_executable(Lox ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Expr.h)
add_executable(Lox ${SOURCES})
