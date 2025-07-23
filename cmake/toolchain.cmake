set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>                   # Force UTF-8 encoding
    $<$<CXX_COMPILER_ID:MSVC>:/MP>                      # Multi process compilation
    $<$<CXX_COMPILER_ID:MSVC>:/wd4251>                  # STL dll interface noise
    $<$<CXX_COMPILER_ID:MSVC>:/wd4275>                  # Base class no dll-interface
    $<$<CXX_COMPILER_ID:MSVC>:/wd4996>                  # "Unsafe" function BS
    $<$<CXX_COMPILER_ID:MSVC>:/wd4100>                  # Unused param
    $<$<CXX_COMPILER_ID:MSVC>:/wd4189>                  # Unused local var
    $<$<CXX_COMPILER_ID:MSVC>:/wd4127>                  # Constant conditional
)

add_compile_definitions(UNICODE _UNICODE)
