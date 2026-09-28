add_library(eeprom STATIC eeprom.c)

target_include_directories(eeprom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# PRIVATE: i2c is a required interface used by eeprom_inc.h, so it stays an
# implementation detail and is not exposed to callers of eeprom_ifa.h.
target_link_libraries(eeprom PRIVATE i2c project_warnings)
