cmake_minimum_required(VERSION 3.10)

project(TempSensorFw C CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug or Release)" FORCE)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# No exceptions, no RTTI
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
                        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
elseif(MSVC)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/GR->)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(WERROR_ENABLE "Treat compiler warnings as errors" ON)
option(SIM_ENABLE "Build the host simulation of the hardware interrupts" ON)

add_library(project_warnings INTERFACE)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(project_warnings INTERFACE
        -Wall -Wextra -Wpedantic
        -Wshadow -Wconversion
        $<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
        $<$<COMPILE_LANGUAGE:C>:-Wmissing-prototypes>
        $<$<COMPILE_LANGUAGE:CXX>:-Wmissing-declarations>
        $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>
        $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
        $<$<COMPILE_LANGUAGE:CXX>:-Wold-style-cast>
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
