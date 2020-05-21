# cats
cmake_minimum_required ( VERSION 3.0 )
project ( "Test" )


set ( TEST_FILES
    Main.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/../Synthe/Synthe/Include
    ${CMAKE_SOURCE_DIR}/System/Include
)

add_executable("Test"
    ${TEST_FILES}
)


target_link_libraries("Test"
    ${CMAKE_BINARY_DIR}/Libs/Synthe.lib
    ${CMAKE_BINARY_DIR}/Libs/System.lib
    dxgi.lib
    d3d12.lib
)