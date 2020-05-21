set ( WIN32_SOURCE_DIR ${SYSTEM_SOURCE_DIR}/Win32 )

set ( SYSTEM_FILES
    ${SYSTEM_FILES}
    ${WIN32_SOURCE_DIR}/Win32Common.hpp
    ${WIN32_SOURCE_DIR}/Win32Display.hpp
    ${WIN32_SOURCE_DIR}/Win32Display.cpp
    ${WIN32_SOURCE_DIR}/Win32Mouse.cpp
    ${WIN32_SOURCE_DIR}/Win32Time.cpp
    ${WIN32_SOURCE_DIR}/Win32WindowCallback.cpp
    ${WIN32_SOURCE_DIR}/Win32WindowCallback.hpp
    ${WIN32_SOURCE_DIR}/Win32WindowProc.cpp
)