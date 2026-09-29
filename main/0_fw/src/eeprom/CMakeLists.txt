add_library(eeprom STATIC eeprom.c)

target_include_directories(eeprom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(eeprom PRIVATE i2c project_warnings)
