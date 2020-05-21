set ( SYNTHE_WIN32_SRC_DIR ${SYNTHE_SOURCE_DIR}/Win32 )

set ( SYNTHE_WIN32_FILES
    ${SYNTHE_WIN32_SRC_DIR}/Win32Common.hpp
)

include_directories(${SYNTHE_WIN32_SRC_DIR})

set ( SYNTHE_GLOB 
    ${SYNTHE_GLOB}
    ${SYNTHE_WIN32_FILES}
)