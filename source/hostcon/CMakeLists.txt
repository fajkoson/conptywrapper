set(target hostcon)
file(GLOB_RECURSE 
    SOURCES 
        CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE 
    HEADERS 
        CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

add_executable(${target} ${SOURCES})

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
