project(Client)

set(UV_MSG_FRAMING_DIR ./deps/libuv_message_framing)
set(ClientFiles client.cpp write.cpp ${UV_MSG_FRAMING_DIR}/uv_msg_framing.c)

include_directories(${UV_MSG_FRAMING_DIR})

add_library(Client ${ClientFiles})

include(${CMAKE_CURRENT_SOURCE_DIR}/FindLibUV.cmake)
target_link_libraries(Client uv)
