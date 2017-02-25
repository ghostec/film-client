project(GUI)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_PREFIX_PATH "/usr/local/opt/qt5/")
find_package(Qt5Widgets)

set(GUIFiles gui.cpp worker.cpp)

add_library(GUI ${GUIFiles})

target_link_libraries(GUI Qt5::Widgets)
