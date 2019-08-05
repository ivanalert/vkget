set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Quick Network Concurrent Multimedia REQUIRED)
include_directories(../vk ../itemmodel)

set(QRC_FILES qml/qml.qrc icons.qrc)
qt5_add_resources(RCC_FILES ${QRC_FILES})

add_executable(vkget WIN32
    main.cpp
    mainwindow.cpp
    mainwindow.h
    navigationlog.h
    utilities.cpp
    utilities.h
    ${RCC_FILES}
)
    
target_compile_definitions(vkget PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(vkget PRIVATE
    Qt5::Core
    Qt5::Quick
    Qt5::Network
    Qt5::Concurrent
    Qt5::Multimedia
    vk
    basicitemmodel
)

# For using in tests executables.
if(TESTS)
    add_library(vkget_static STATIC
        mainwindow.cpp
        mainwindow.h
        navigationlog.h
        utilities.cpp
        utilities.h
        ${RCC_FILES}
    )

    target_link_libraries(vkget_static PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Network
        Qt5::Concurrent
        Qt5::Multimedia
        vk
        basicitemmodel
    )
endif()
