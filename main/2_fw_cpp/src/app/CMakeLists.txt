add_library(app_cpp STATIC AppOrchestrator.cpp app_dat.cpp)

target_include_directories(app_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app_cpp
    PUBLIC
        adc_adapt
        dma_adapt
        tim_adapt
        eeprom_cpp
        led_cpp
        temp_sensor_cpp
    PRIVATE
        gpio_adapt
        i2c_adapt
        platform
        project_warnings
)
