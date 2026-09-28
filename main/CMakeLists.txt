# Layers are added bottom-up: platform sits below the BSP, which knows nothing
# about the firmware above it.
add_subdirectory(platform)
add_subdirectory(1_bsp)
add_subdirectory(0_fw)

if(SIM_ENABLE)
    add_subdirectory(4_sim)
endif()
