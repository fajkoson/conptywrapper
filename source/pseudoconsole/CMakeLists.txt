set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(target pseudoconsole)
file(GLOB_RECURSE 
    SOURCES 
        CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE 
    HEADERS 
        CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

add_library(${target}
    MODULE 
        ${HEADERS}
        ${SOURCES}
)

target_include_directories(${target}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/3rd-party/include
        ${Python3_INCLUDE_DIRS}
)

target_link_libraries(${target}
    PRIVATE 
        ${Python3_LIBRARIES} 
)

set_target_properties(${target} 
    PROPERTIES
        PREFIX ""
        SUFFIX ".pyd"
)

target_compile_definitions(${target} 
    PRIVATE
        -D_WIN32_WINNT=0x0A00
)

source_group("src" 
    FILES 
        ${SOURCES}
)
source_group("inc" 
    FILES 
        ${HEADERS})
