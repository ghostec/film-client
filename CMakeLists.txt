cmake_minimum_required(VERSION 3.6)
project(film_client)

set(CMAKE_CXX_STANDARD 14)

include_directories(./ ./deps)

add_subdirectory(./deps/film-network)
add_subdirectory(gui)

set(LIBRARIES Film_Network GUI)

add_executable(film_client main.cpp)

target_link_libraries(film_client ${LIBRARIES})
