cmake_minimum_required(VERSION 2.8.8)
project(wubwub)
set(w_QT5_CMAKES CACHE FILEPATH "QT5 Cmake files (qtbase/lib/cmake)")
set(w_TAGLIB_DIR CACHE FILEPATH "taglib path")
set(w_BASS_DIR CACHE FILEPATH "bass path")
set(CMAKE_PREFIX_PATH ${w_QT5_CMAKES})

if(w_CMAKE_PREFIX_PATH STREQUAL "")
    message(FATAL_ERROR "Must provide qt5 cmake files directory. It's qtbase/lib/cmake in wherever you have qt5")
endif()

IF(WIN32)
    # Fix stupid macro problem
    add_definitions(-DNOMINMAX)
    set(w_GUI WIN32)
    set(w_LIBS Qt5::WinMain)
    set(w_INPUT "../res/ssmp.rc")
ENDIF(WIN32)
IF(APPLE)
    SET(w_GUI MACOSX_BUNDLE)
ENDIF(APPLE)
###### QT
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5OpenGL REQUIRED)
# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
add_definitions(${Qt5Widgets_DEFINITIONS})
######
###### TAGLIB
find_library(TAGLIB_LIB tag tag_dll
   PATHS ${w_TAGLIB_DIR}/taglib
       PATH_SUFFIXES ${CMAKE_BUILD_TYPE})
include_directories(${w_TAGLIB_DIR})
include_directories(${w_TAGLIB_DIR}/taglib)
include_directories(${w_TAGLIB_DIR}/taglib/ogg)
include_directories(${w_TAGLIB_DIR}/taglib/toolkit)
######
###### BASS
find_library(BASS_LIB bass bass_dll
    PATHS ${w_BASS_DIR})
include_directories(${w_BASS_DIR})
######
###### OPENGL
find_library(OPENGL_LIB opengl32 opengl32_dll)
######

file(GLOB_RECURSE wubwub_SOURCES "*.cpp")
file(GLOB_RECURSE wubwub_HEADERS "*.h")
file(GLOB_RECURSE wubwub_UI "*.ui")
set(wubwub_QRC "./../res/res.qrc") #Not sure why this needs a leading . but it does

set (wubwub_INCLUDE_DIRS "")
foreach (_headerFile ${wubwub_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND wubwub_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES wubwub_INCLUDE_DIRS)

include_directories(${wubwub_INCLUDE_DIRS})
qt5_wrap_ui(wubwub_FORMS ${wubwub_UI})
# RC files need to be processed by Qt wrapper to generate RC headers.
qt5_add_resources(wubwub_RESOURCES ${wubwub_QRC})
set(w_INPUT ${w_GUI} ${w_INPUT} ${wubwub_SOURCES} ${wubwub_FORMS} ${wubwub_RESOURCES})
add_executable(wubwub ${w_INPUT})

set(w_LIBS ${w_LIBS} 
    Qt5::OpenGL Qt5::Widgets 
    ${TAGLIB_LIB} ${OPENGL_LIB} ${BASS_LIB})
target_link_libraries(wubwub ${w_LIBS})
# Setup qt5 modules
qt5_use_modules(wubwub Widgets Svg Sql OpenGL)
