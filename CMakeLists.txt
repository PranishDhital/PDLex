cmake_minimum_required(VERSION 3.20)
project(PDLex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────────
# Output directories (IMPORTANT for Visual Studio)
# ─────────────────────────────────────────────

# This is REQUIRED for multi-config generators (like Visual Studio)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG_UPPER)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        ${CMAKE_BINARY_DIR}/bin/${CONFIG}
    )
endforeach()

# ─────────────────────────────────────────────
# Source files
# ─────────────────────────────────────────────

add_executable(ppi
    main.cpp
    ast/AST.cpp
    lexer/lexer.cpp
    interpreter/interpreter.cpp
    error/error.cpp
)

# ─────────────────────────────────────────────
# Include directories
# ─────────────────────────────────────────────

target_include_directories(ppi PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ast
    ${CMAKE_SOURCE_DIR}/lexer
    ${CMAKE_SOURCE_DIR}/interpreter
    ${CMAKE_SOURCE_DIR}/error
)