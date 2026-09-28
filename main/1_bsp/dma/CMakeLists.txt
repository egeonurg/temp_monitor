add_library(dma STATIC dma.c)

target_include_directories(dma PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dma PRIVATE platform project_warnings)
