cmake_minimum_required(VERSION 3.28)

project(Staff_Flashcard LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_compile_options(-Wall -Wextra -Wpedantic -pg)

add_executable(StaffFlashcard 
    src/main.cpp 
    src/button.cpp 
    src/checkbox.cpp 
    src/audioSetup.cpp
    src/staff.cpp 
    src/staffSetup.cpp 
    src/types.cpp 
    src/audioProcessor.cpp)

target_link_libraries(StaffFlashcard PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

target_include_directories(StaffFlashcard PRIVATE ${CMAKE_SOURCE_DIR}/src)

configure_file("${CMAKE_SOURCE_DIR}/resource/font.ttf" "${CMAKE_BINARY_DIR}/bin/font.ttf" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/resource/saved_device.dat" "${CMAKE_BINARY_DIR}/bin/saved_device.dat" COPYONLY)
