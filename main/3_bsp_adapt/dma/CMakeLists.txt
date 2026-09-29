add_library(dma_adapt STATIC dma.cpp)

target_include_directories(dma_adapt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dma_adapt PRIVATE dma project_warnings)
