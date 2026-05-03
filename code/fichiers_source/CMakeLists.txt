cmake_minimum_required(VERSION 3.16)

project(VillageQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

set(SIMULATION_SOURCES
    simulation/Village.cpp
    simulation/Batiment.cpp
    simulation/Villageois.cpp
    simulation/Champ.cpp
    simulation/Taverne.cpp
    simulation/Ressource.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VillageQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        simulation/Villageois.hpp
        simulation/Villageois.cpp

        simulation/Village.hpp
        simulation/Village.cpp
        simulation/Taverne.hpp
        simulation/Taverne.cpp
        simulation/Ressource.cpp
        simulation/Champ.hpp
        simulation/Champ.cpp
        simulation/Batiment.cpp
        qtstreambuf.h qtstreambuf.cpp
        simulation/Moulin.hpp simulation/Moulin.cpp
        simulation/Boulangerie.hpp simulation/Boulangerie.cpp
        assets.qrc
        simulation/Paturage.hpp simulation/Paturage.cpp
        simulation/Batiment.hpp
        simulation/Logger.hpp
        simulation/Mine.hpp simulation/Mine.cpp
        simulation/Forge.hpp simulation/Forge.cpp
        simulation/Abbaye.hpp simulation/Abbaye.cpp
        simulation/Manoir.hpp simulation/Manoir.cpp



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET VillageQt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(VillageQt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(VillageQt
            ${PROJECT_SOURCES}
            ${SIMULATION_SOURCES}
        )
    endif()
endif()

target_link_libraries(VillageQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(VillageQt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/simulation
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VillageQt)
endif()
set_target_properties(VillageQt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS VillageQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VillageQt)
endif()
