cmake_minimum_required(VERSION 3.16)
project(IN104)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/raylib/include)

file(GLOB SOURCES "src/*.c")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
link_directories(${CMAKE_SOURCE_DIR}/raylib/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(main ${SOURCES})
#[[so that no console window is opened]]
#add_executable(main WIN32 ${SOURCES})
if(WIN32)
    target_link_libraries(main raylib winmm)
else()
    target_link_libraries(main raylib)
endif()