# CMake build script.
cmake_minimum_required ( VERSION 3.0 )
project ( "System" )

set ( SYSTEM_NAME "System" )
set ( SYSTEM_INCLUDE_DIR Include )
set ( SYSTEM_SOURCE_DIR Source )

set ( SYSTEM_LIBS )
set ( SYSTEM_INCLUDE_DIRS )
set ( SYSTEM_FILES )

set ( SYSTEM_INCLUDE_DIRS
    ${SYSTEM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/Synthe/Include 
)

message ( STATUS "Include Dir: ${SYSTEM_INCLUDE_DIRS}" )
include ( CMake/Input.cmake )
include ( CMake/Display.cmake )
include ( CMake/System.cmake )
include ( CMake/Win32.cmake )

add_library (
    ${SYSTEM_NAME} STATIC
    ${SYSTEM_FILES}
)

target_include_directories(${SYSTEM_NAME} PUBLIC
    SYSTEM ${SYSTEM_INCLUDE_DIR}
    SYSTEM ${SYSTEM_INCLUDE_DIRS}
)

install(
    TARGETS ${SYSTEM_NAME}
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/Libs
    ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/Libs
)

# Doing some stuff for organization.
if (MSVC)
  foreach(source IN LISTS SYSTEM_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
  endforeach()
endif()

# Visual Studio organization. For Good purposes.
if (WIN32)
  set_property(TARGET ${SYSTEM_NAME} 
    PROPERTY 
    FOLDER ${SYSTEM_NAME})
endif()