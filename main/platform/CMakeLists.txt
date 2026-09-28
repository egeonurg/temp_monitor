# Header-only abstraction shared by every layer: asserts, logging, and later
# traps and critical sections. It sits below the BSP so drivers can use it
# without depending upwards on the firmware layer.
add_library(platform INTERFACE)

target_include_directories(platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Desktop simulation build: this is what makes PLATFORM_LOG print.
if(SIM_ENABLE)
    target_compile_definitions(platform INTERFACE SIM_ENABLE)
endif()
