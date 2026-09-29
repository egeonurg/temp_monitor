add_library(adc_adapt STATIC adc.cpp)

target_include_directories(adc_adapt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(adc_adapt PRIVATE adc project_warnings)
