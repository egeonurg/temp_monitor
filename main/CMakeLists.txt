add_subdirectory(platform)
add_subdirectory(1_bsp)
add_subdirectory(3_bsp_adapt)
add_subdirectory(0_fw)
add_subdirectory(2_fw_cpp)

if(SIM_ENABLE)
    add_subdirectory(6_sim)
endif()
