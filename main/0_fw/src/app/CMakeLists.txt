add_library(app STATIC app.c)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PRIVATE eeprom temp_sensor tim adc dma led platform project_warnings)
