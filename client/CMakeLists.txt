project(Client)

set(ClientFiles client.cpp observable.cpp write.cpp)

add_library(Client ${ClientFiles})

include(${CMAKE_CURRENT_SOURCE_DIR}/FindLibUV.cmake)
target_link_libraries(Client uv)

set_property(TARGET Client PROPERTY CXX_STANDARD 14)
