cmake_minimum_required(VERSION 3.5)

project(Staff_Flashcard)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

add_executable(StaffFlashcard 
    src/main.cpp 
    src/button.cpp 
    src/audioSetup.cpp
    src/staff.cpp 
    src/staffSetup.cpp 
    src/types.cpp 
    src/audioProcessor.cpp)


target_link_libraries(StaffFlashcard sfml-graphics sfml-window sfml-system sfml-audio)

target_include_directories(StaffFlashcard PRIVATE ${CMAKE_SOURCE_DIR}/src)

configure_file("${CMAKE_SOURCE_DIR}/resource/font.ttf" "${CMAKE_BINARY_DIR}/font.ttf" COPYONLY)
