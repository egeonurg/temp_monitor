add_library(sim STATIC sim.c)

target_include_directories(sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sim PRIVATE dma tim platform project_warnings)
