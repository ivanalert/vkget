set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Network Test REQUIRED)
include_directories(../vk ../itemmodel ../app)

add_executable(testbasicitemmodel
    testbasicitemmodel.cpp
)

target_link_libraries(testbasicitemmodel PRIVATE
    Qt5::Gui
    Qt5::Test
    pthread
    basicitemmodel
)

add_executable(testvk
    testvk.cpp
)

target_link_libraries(testvk PRIVATE
    Qt5::Core
    Qt5::Network
    Qt5::Test
    vk
)

add_executable(testmainwindow
    testmainwindow.cpp
)

target_link_libraries(testmainwindow PRIVATE
    Qt5::Gui
    Qt5::Test
    vkget_static
)
