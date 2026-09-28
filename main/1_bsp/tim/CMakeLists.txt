add_library(tim STATIC tim.c)

target_include_directories(tim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tim PRIVATE platform project_warnings)
