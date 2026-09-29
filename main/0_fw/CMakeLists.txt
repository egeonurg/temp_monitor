add_subdirectory(src)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE
    app
    project_warnings
)

if(SIM_ENABLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE sim)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SIM_ENABLE)
endif()
