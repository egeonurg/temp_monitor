add_subdirectory(comp)

add_executable(${PROJECT_NAME} main.c)

# app performs the start-up wiring. main talks to app only: every driver
# below it is app's private dependency.
target_link_libraries(${PROJECT_NAME} PRIVATE
    app
    project_warnings
)

# Host build only: the simulator raises the 1 ms interrupt that TIM0 raises on
# target, and bounds the superloop so the program terminates.
if(SIM_ENABLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE sim)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SIM_ENABLE)
endif()
