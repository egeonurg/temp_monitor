add_library(led STATIC led.c)

target_include_directories(led PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(led PRIVATE gpio platform project_warnings)
