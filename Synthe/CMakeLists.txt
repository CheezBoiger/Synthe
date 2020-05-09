# Copyright (c) 2020 Mario Garcia.
set ( SYNTHE_NAME "Synthe" )

set ( SYNTHE_MAJOR 0 )
set ( SYNTHE_MINOR 0 )
set ( SYNTHE_PATCH 0 )

set ( SYNTHE_VERSION_STRING "${SYNTHE_MAJOR}_${SYNTHE_MINOR}_${SYNTHE_PATH}" )
set ( SYNTHE_NAME_STRING "${SYNTHE_NAME}_v${SYNTHE_NAME_STRING}" )


set ( SYNTHE_INCLUDE_DIR Include )
set ( SYNTHE_SOURCE_DIR Source )

set ( SYNTHE_GLOB )
set ( SYNTHE_LIBS )


include ( CMake/Graphics.cmake )
include ( CMake/Input.cmake )


add_library(${SYNTHE_NAME} STATIC 
    ${SYNTHE_GLOB}
)

target_link_libraries(
    ${SYNTHE_NAME}
    ${SYNTHE_LIBS}
)