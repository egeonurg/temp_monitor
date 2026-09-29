add_library(eeprom_cpp STATIC EepromController.cpp)

target_include_directories(eeprom_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(eeprom_cpp PRIVATE i2c_adapt project_warnings)
