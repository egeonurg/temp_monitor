add_subdirectory(src)

add_executable(TempSensorFwCpp main.cpp)

target_link_libraries(TempSensorFwCpp PRIVATE
    app_cpp
    project_warnings
)

if(SIM_ENABLE)
    target_link_libraries(TempSensorFwCpp PRIVATE sim)
    target_compile_definitions(TempSensorFwCpp PRIVATE SIM_ENABLE)
endif()
