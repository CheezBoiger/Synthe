set ( SYNTHE_COMMON_INC_DIR ${SYNTHE_INCLUDE_DIR}/Common )
set ( SYNTHE_COMMON_INC_MATH_DIR ${SYNTHE_COMMON_INC_DIR}/Math )
set ( SYNTHE_MATH_SRC_DIR ${SYNTHE_SOURCE_DIR}/Common/Math )
set ( SYNTHE_MEMORY_INC_DIR ${SYNTHE_COMMON_INC_DIR}/Memory )
set ( SYNTHE_SYSTEM_INC_DIR ${SYNTHE_COMMON_INC_DIR}/System )
set ( SYNTHE_MEMORY_SRC_DIR ${SYNTHE_SOURCE_DIR}/Memory )


set ( SYNTHE_MATH_FILES
    ${SYNTHE_COMMON_INC_MATH_DIR}/Matrix22.hpp
    ${SYNTHE_COMMON_INC_MATH_DIR}/Matrix33.hpp
    ${SYNTHE_COMMON_INC_MATH_DIR}/Matrix44.hpp
    ${SYNTHE_COMMON_INC_MATH_DIR}/Vector2.hpp
    ${SYNTHE_COMMON_INC_MATH_DIR}/Vector3.hpp
    ${SYNTHE_COMMON_INC_MATH_DIR}/Vector4.hpp
)

set ( SYNTHE_MEMORY_FILES
    ${SYNTHE_MEMORY_INC_DIR}/Allocator.hpp
    ${SYNTHE_MEMORY_INC_DIR}/LinearAllocator.hpp
    ${SYNTHE_MEMORY_INC_DIR}/NewAllocator.hpp
    ${SYNTHE_MEMORY_INC_DIR}/BuddyAllocator.hpp
    ${SYNTHE_MEMORY_INC_DIR}/FreeListAllocator.hpp
    
    ${SYNTHE_MEMORY_SRC_DIR}/LinearAllocator.cpp
    ${SYNTHE_MEMORY_SRC_DIR}/NewAllocator.cpp
    ${SYNTHE_MEMORY_SRC_DIR}/BuddyAllocator.cpp
    ${SYNTHE_MEMORY_SRC_DIR}/FreeListAllocator.cpp
)

set ( SYNTHE_COMMON_FILES
    ${SYNTHE_COMMON_INC_DIR}/Types.hpp
    ${SYNTHE_COMMON_INC_DIR}/String.hpp
)


set ( SYNTHE_GLOB
    ${SYNTHE_GLOB}
    ${SYNTHE_COMMON_FILES}
    ${SYNTHE_MEMORY_FILES}
    ${SYNTHE_MATH_FILES}
)