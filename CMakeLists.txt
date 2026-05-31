cmake_minimum_required(VERSION 3.20)
project(PDLex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── output inside build folder ─────────────────────────────

# Default (fallback)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Debug → build/Debug
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
        ${CMAKE_BINARY_DIR}/Debug
)

# Release → build/Release
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE
        ${CMAKE_BINARY_DIR}/Release
)

add_executable(ppi
        main.cpp
        ast/AST.cpp
        lexer/lexer.cpp
        interpreter/interpreter.cpp
        error/error.cpp
)

target_include_directories(ppi PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/ast
        ${CMAKE_SOURCE_DIR}/lexer
        ${CMAKE_SOURCE_DIR}/interpreter
)