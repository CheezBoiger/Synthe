# Copyright (c) 2020 Mario Garcia.
cmake_minimum_required ( VERSION 3.0 )
project ( "Synthe" )
set ( SYNTHE_NAME "Synthe" )

set ( SYNTHE_MAJOR 0 )
set ( SYNTHE_MINOR 0 )
set ( SYNTHE_PATCH 0 )

set ( SYNTHE_VERSION_STRING "${SYNTHE_MAJOR}_${SYNTHE_MINOR}_${SYNTHE_PATCH}" )
set ( SYNTHE_NAME_STRING "${SYNTHE_NAME}_v${SYNTHE_VERSION_STRING}" )


set ( SYNTHE_INCLUDE_DIR Include )
set ( SYNTHE_SOURCE_DIR Source )

set ( SYNTHE_INCLUDE_DIRS ${SYNTHE_INCLUDE_DIR})
set ( SYNTHE_GLOB )
set ( SYNTHE_LIBS )

add_definitions( 
    -DSYNTHE_VERSION_STR="${SYNTHE_VERSION_STRING}"
    -DSYNTHE_NAME_STR="${SYNTHE_NAME_STRING}"
    -DSYNTHE_MAJOR=${SYNTHE_MAJOR}
    -DSYNTHE_MINOR=${SYNTHE_MINOR}
    -DSYNTHE_PATCH=${SYNTHE_PATCH}
)

include ( CMake/Common.cmake )
include ( CMake/Graphics.cmake )
include ( CMake/Input.cmake )
include ( CMake/Win32.cmake )

include_directories(${SYNTHE_INCLUDE_DIRS})

add_library(${SYNTHE_NAME} STATIC 
    ${SYNTHE_GLOB}
)

target_link_libraries(
    ${SYNTHE_NAME}
    ${SYNTHE_LIBS}
)

install(
        TARGETS ${SYNTHE_NAME} 
        LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/Libs
        ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/Libs
)

# Doing some stuff for organization.
if (MSVC)
  foreach(source IN LISTS SYNTHE_GLOB)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
  endforeach()
endif()

# Visual Studio organization. For Good purposes.
if (WIN32)
  set_property(TARGET ${SYNTHE_NAME} 
    PROPERTY 
    FOLDER ${SYNTHE_NAME})
endif()