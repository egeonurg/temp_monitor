add_library(app_cpp STATIC AppOrchestrator.cpp app_dat.cpp)

target_include_directories(app_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app_cpp
    PUBLIC
        temp_sensor_cpp
    PRIVATE
        adc_adapt
        dma_adapt
        tim_adapt
        gpio_adapt
        i2c_adapt
        eeprom_cpp
        led_cpp
        platform
        project_warnings
)
