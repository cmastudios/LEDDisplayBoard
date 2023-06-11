add_library(images OBJECT ${CMAKE_CURRENT_BINARY_DIR}/images.c ${CMAKE_CURRENT_BINARY_DIR}/images.h)

set(IMAGES
    ${CMAKE_CURRENT_SOURCE_DIR}/face.png
    ${CMAKE_CURRENT_SOURCE_DIR}/happy.png
    ${CMAKE_CURRENT_SOURCE_DIR}/love.png
    ${CMAKE_CURRENT_SOURCE_DIR}/scared.png
    ${CMAKE_CURRENT_SOURCE_DIR}/surprised.png
    ${CMAKE_CURRENT_SOURCE_DIR}/tired.png
    ${CMAKE_CURRENT_SOURCE_DIR}/uwu.png
    ${CMAKE_CURRENT_SOURCE_DIR}/wink.png
    ${CMAKE_CURRENT_SOURCE_DIR}/wirelesserror.png
    ${CMAKE_CURRENT_SOURCE_DIR}/lowbattery.png
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/images.c ${CMAKE_CURRENT_BINARY_DIR}/images.h
    COMMAND 
        python ${CMAKE_CURRENT_SOURCE_DIR}/build.py
        ${CMAKE_CURRENT_BINARY_DIR}/images.c
        ${CMAKE_CURRENT_BINARY_DIR}/images.h
        ${IMAGES}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/build.py
        ${IMAGES}
)

target_include_directories(images PUBLIC ${CMAKE_CURRENT_BINARY_DIR})