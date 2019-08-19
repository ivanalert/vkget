set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 Network Concurrent Multimedia REQUIRED)
include_directories(../vk ../itemmodel)

set(QRC_FILES qml/qml.qrc icons.qrc)
qt5_add_resources(RCC_FILES ${QRC_FILES})

SET(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    navigationlog.h
    utilities.cpp
    utilities.h
    taskgroup.cpp
    taskgroup.h
    task.cpp
    task.h
)

if (WIN32)
    add_executable(vkget WIN32
    ${SOURCES}
    ${RCC_FILES}
    )
else()
    add_executable(vkget
    ${SOURCES}
    ${RCC_FILES}
    )
endif()

target_compile_definitions(vkget PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(vkget PRIVATE
    Qt5::Core
    Qt5::Quick
    Qt5::QuickControls2
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
        Qt5::QuickControls2
        Qt5::Network
        Qt5::Concurrent
        Qt5::Multimedia
        vk
        basicitemmodel
    )
endif()
