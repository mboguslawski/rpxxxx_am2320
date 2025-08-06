add_library(rpxxxx_am2320 STATIC
    rpxxxx_am2320.c
)

target_link_libraries(rpxxxx_am2320
        hardware_i2c
        pico_stdlib
)

target_include_directories(rpxxxx_am2320 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
