cmake_minimum_required(VERSION 3.1)

project(VKGet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output.
set(BINARY_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_DIR}/lib")

set(QML ON)
set(WEBVIEW CHROMIUM)

# App and library.
add_subdirectory(app)
add_subdirectory(vk)
add_subdirectory(itemmodel)
# Tests dir.
if(TESTS)
    add_subdirectory(tests)
endif()
