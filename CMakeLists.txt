cmake_minimum_required(VERSION 3.21)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)

add_executable(photo_segment main.cpp new_watershed.cpp on_Mouse.cpp helptext.cpp grab_cut.cpp globevary.cpp)

target_include_directories(photo_segment PUBLIC ${OpenCV_INCLUDE_DIRS})

include_directories(photo_segment  include)

target_link_libraries(photo_segment   ${OpenCV_LIBS})
