cmake_minimum_required(VERSION 3.10)

project(TempSensorFw C)

# Debug by default: without a build type there are no -g flags and the binary
# cannot be stepped through.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug or Release)" FORCE)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Keep the executable at a stable path (build/bin) regardless of the source tree
# layout, so the helper scripts and the VS Code launch config do not need to
# track where the target is defined.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(WERROR_ENABLE "Treat compiler warnings as errors" ON)
option(SIM_ENABLE "Build the host simulation of the hardware interrupts" ON)

# Warning set shared by every component; link it PRIVATE from each target.
add_library(project_warnings INTERFACE)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(project_warnings INTERFACE
        -Wall -Wextra -Wpedantic
        -Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes
    )
    if(WERROR_ENABLE)
        target_compile_options(project_warnings INTERFACE -Werror)
    endif()
elseif(MSVC)
    target_compile_options(project_warnings INTERFACE /W4)
    if(WERROR_ENABLE)
        target_compile_options(project_warnings INTERFACE /WX)
    endif()
endif()

add_subdirectory(main)
