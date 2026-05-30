cmake_minimum_required(VERSION 3.20)
project(PDLex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── output exe to project root ────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

add_executable(ppi
    main.cpp
    ast/AST.cpp
    lexer/lexer.cpp
    interperter/interpreter.cpp
 "error/error.h" "error/error.cpp")

target_include_directories(ppi PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ast
    ${CMAKE_SOURCE_DIR}/lexer
    ${CMAKE_SOURCE_DIR}/interperter
)