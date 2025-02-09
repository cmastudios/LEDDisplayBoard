set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

add_library(samc21j18a OBJECT
    startup_samc21j18a.c
    system_samc21j18a.c
)

target_compile_definitions(samc21j18a
    PUBLIC __SAMC21J18A__
)

target_include_directories(samc21j18a
    SYSTEM PUBLIC CMSIS/Core/Include
                  CMSIS/Include
                  include
)

target_compile_options(samc21j18a
    PRIVATE -w
)

target_link_options(samc21j18a
    PUBLIC -T ${CMAKE_CURRENT_SOURCE_DIR}/samc21j18a_flash.ld
)