cmake_minimum_required(VERSION 3.10)
project(LineForge VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files for the main application
set(SOURCES
    src/main.cpp
    src/DoublyLinkedList.cpp
    src/Document.cpp
    src/Command.cpp
    src/UndoRedoManager.cpp
    src/SearchEngine.cpp
    src/FileManager.cpp
    src/Editor.cpp
    src/UserInterface.cpp
)

add_executable(lineforge ${SOURCES})

# Warnings
if(MSVC)
    target_compile_options(lineforge PRIVATE /W4)
else()
    target_compile_options(lineforge PRIVATE -Wall -Wextra -pedantic)
endif()

# Test executable (simple custom test runner)
set(TEST_SOURCES
    tests/test_main.cpp
    src/DoublyLinkedList.cpp
    src/Document.cpp
    src/Command.cpp
    src/UndoRedoManager.cpp
    src/SearchEngine.cpp
    src/FileManager.cpp
    src/Editor.cpp
)

add_executable(lineforge_tests ${TEST_SOURCES})
target_include_directories(lineforge_tests PRIVATE src)

if(MSVC)
    target_compile_options(lineforge_tests PRIVATE /W4)
else()
    target_compile_options(lineforge_tests PRIVATE -Wall -Wextra -pedantic)
endif()

# Install rules (optional)
install(TARGETS lineforge DESTINATION bin)

enable_testing()
add_test(NAME LineForgeTests COMMAND lineforge_tests)
