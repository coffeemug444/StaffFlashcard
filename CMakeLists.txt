cmake_minimum_required(VERSION 3.5)

project(Staff_Flashcard)

set (CMAKE_CXX_STANDARD 23)

add_executable(StaffFlashcard main.cpp staff.cpp types.cpp audioProcessor.cpp)

target_link_libraries(StaffFlashcard sfml-graphics sfml-window sfml-system sfml-audio)

configure_file("${CMAKE_SOURCE_DIR}/font.ttf" "${CMAKE_BINARY_DIR}/font.ttf" COPYONLY)