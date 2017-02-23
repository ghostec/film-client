project(GUI)

set(CMAKE_PREFIX_PATH "/usr/local/opt/qt5/")
find_package(Qt5Widgets)

set(GUIFiles gui.cpp)

add_library(GUI ${GUIFiles})

target_link_libraries(GUI Qt5::Widgets)
