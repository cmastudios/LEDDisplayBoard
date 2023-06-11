cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-none-eabi.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(rinaboot LANGUAGES ASM C CXX)


add_subdirectory(external/atsamc21j18a)
add_subdirectory(images)


add_executable(${PROJECT_NAME}.elf
	$<TARGET_OBJECTS:samc21j18a>
	$<TARGET_OBJECTS:images>
	main.c
	drv_nrf24l01.c
	drv_ws2812b.c
)



target_link_libraries(${PROJECT_NAME}.elf
    PUBLIC samc21j18a images
)


#add_custom_command(TARGET ${PROJECT_NAME}.elf
#	POST_BUILD
#	COMMAND                     ${CMAKE_OBJCOPY} -O ihex -R .eeprom -R .fuse -R .lock -R .signature ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
#	WORKING_DIRECTORY           ${CMAKE_CURRENT_BINARY_DIR}
#)

include(binutils-arm-none-eabi)

print_section_sizes(${PROJECT_NAME}.elf)
create_hex_output(${PROJECT_NAME}.elf)