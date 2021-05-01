cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
project(Tetris)

find_package(Curses REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
include_directories(${CURSES_INCLUDE_DIRS})

add_library(${PROJECT_NAME}_lib STATIC
    src/tetris.cpp
    src/board.cpp
)

target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}_lib
        ${CURSES_LIBRARIES}
)
