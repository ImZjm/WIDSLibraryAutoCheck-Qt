cmake_minimum_required(VERSION 3.16)

project(WIDSLibraryAutoCheck VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(OpenSSL REQUIRED)

option(USE_OPENSSL "OPENSSL" ON)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WIDSLibraryAutoCheck
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        core/datacontrol.h core/datacontrol.cpp
        pojo/user.h pojo/user.cpp
        pojo/authconfig.h pojo/authconfig.cpp
        core/checkcore.h core/checkcore.cpp
        ui/logindialog.h ui/logindialog.cpp ui/logindialog.ui
        libs/cpp-httplib/httplib.h
        core/apirequest.h core/apirequest.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WIDSLibraryAutoCheck APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(WIDSLibraryAutoCheck SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WIDSLibraryAutoCheck
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(WIDSLibraryAutoCheck PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WIDSLibraryAutoCheck)
endif()
set_target_properties(WIDSLibraryAutoCheck PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WIDSLibraryAutoCheck
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WIDSLibraryAutoCheck)
endif()

include_directories(libs/cpp-httplib)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

if(USE_OPENSSL)
    add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()


