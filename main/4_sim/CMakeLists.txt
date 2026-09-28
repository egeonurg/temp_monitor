# Host-only simulation of the interrupts the target gets from hardware. It
# drives the BSP drivers directly and knows nothing about the layers above.
add_library(sim STATIC sim.c)

target_include_directories(sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# temp_sensor is linked for its resolution only: the simulated converter has
# to produce counts in the unit the fitted revision uses.
target_link_libraries(sim PRIVATE dma tim temp_sensor platform project_warnings)
