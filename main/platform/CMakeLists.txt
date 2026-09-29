add_library(platform INTERFACE)

target_include_directories(platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(SIM_ENABLE)
    target_compile_definitions(platform INTERFACE SIM_ENABLE)
endif()
