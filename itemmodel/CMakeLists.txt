set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_library(basicitemmodel STATIC
    basicitemmodel.cpp
    basicitemmodel.h
    basicitem.cpp
    basicitem.h
    threadsafenode.h
)

target_link_libraries(basicitemmodel PRIVATE
    Qt5::Core
    Qt5::Gui
)
