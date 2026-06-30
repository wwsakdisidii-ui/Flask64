cmake_minimum_required(VERSION 3.15)
project(N64Emulator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Locate External Dependencies (SDL2 handles windowing and input for the .exe)
find_package(SDL2 REQUIRED)

# 2. Include directory paths
include_directories(src external)

# 3. Collect all source files
file(GLOB SOURCES "src/*.cpp" "src/*.hpp")

# 4. Define the executable target output (.exe on Windows)
add_executable(${PROJECT_NAME} ${SOURCES})

# 5. Link libraries into the final executable
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
