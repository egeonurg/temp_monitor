add_library(app STATIC app.c)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# The integration layer is the only component allowed to know about both
# drivers; they stay PRIVATE so nothing above app inherits them.
target_link_libraries(app PRIVATE eeprom temp_sensor tim adc dma platform project_warnings)
