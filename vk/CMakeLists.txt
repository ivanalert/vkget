set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Choose libs for authorization.
if(WEBVIEW)
    if(QML)
        # Use QtWebView for authorization.
        if(${WEBVIEW} STREQUAL "NATIVE")
            set(QT_COMPONENTS Core Quick WebView)
            add_definitions(-DNATIVE_QML)
        # Use QtWebEngineView for authorization.
        elseif(${WEBVIEW} STREQUAL "CHROMIUM")
            set(QT_COMPONENTS Core Quick WebEngine)
            add_definitions(-DCHROMIUM_QML)
        else()
            message(SEND_ERROR "wrong WEBVIEW value, use NATIVE or CHROMIUM")
        endif()
        set(VK_LOGIN_DIALOG vklogindialogqml.h vklogindialogqml.cpp)
        configure_file(vklogindialogqml.h "${BINARY_DIR}/lib/vklogindialogqml.h" COPYONLY)
    else()
        # Use QWebView for authorization.
        if(${WEBVIEW} STREQUAL "WEBKIT")
            set(QT_COMPONENTS Widgets WebKitWidgets)
            add_definitions(-DWEBKIT)
        # Use QWebEngineView for authorization.
        elseif(${WEBVIEW} STREQUAL "CHROMIUM")
            set(QT_COMPONENTS Widgets WebEngineWidgets)
            add_definitions(-DCHROMIUM)
        else()
            message(SEND_ERROR "wrong WEBVIEW value, use WEBKIT or CHROMIUM")
        endif()
        set(VK_LOGIN_DIALOG vklogindialog.h vklogindialog.cpp)
        configure_file(vklogindialog.h "${BINARY_DIR}/lib/vklogindialog.h" COPYONLY)
    endif()
else()
    # Use QtNetworkAuth for authorization (later).
    set(QT_LIBS NetworkAuth)
endif()

find_package(Qt5 COMPONENTS ${QT_COMPONENTS} Network REQUIRED)
foreach(COMPONENT ${QT_COMPONENTS})
    list(APPEND QT_LIBS Qt5::${COMPONENT})
endforeach()

include_directories(../itemmodel)

add_library(vk STATIC
    vkontakte.h
    vkontakte.cpp
    vkresponse.h
    vkresponse.cpp
    vkitemmodel.h
    vkitemmodel.cpp
    vkitem.h
    sectionmanager.cpp
    sectionmanager.h
    playlist.cpp
    playlist.h
    downloadmanager.cpp
    downloadmanager.h
    ${VK_LOGIN_DIALOG}
)

target_link_libraries(vk PRIVATE
    Qt5::Network
    ${QT_LIBS}
    basicitemmodel
)
