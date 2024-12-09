cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Generic) #Cross compiling for bare metal
set(CMAKE_SYSTEM_PROCESSOR ARM) 
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)

# Project name
project(app LANGUAGES C ASM)

set(MCU_FLAGS "-mcpu=cortex-m3 --specs=nano.specs --specs=nosys.specs -mthumb")
set(CMAKE_C_FLAGS "${MCU_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -Wall -Wextra -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS_RELEASE "-O0")
 
set(CMAKE_BUILD_TYPE "Debug")

# Include path
include_directories(
	CMSIS/inc/
    Code
    LibPer/inc
)
 
# Search *.c files
file(GLOB CMSIS_src CMSIS/src/*.c)
file(GLOB LibPer_src LibPer/src/*.c)
file(GLOB Code_src Code/*.c)
 
# Drivers
add_library(
	user_lib
	${CMSIS_src}
    ${LibPer_src}
)
# main source file
add_library(
	user_code
	${Code_src}
)
 
# startup file
enable_language(ASM)
add_library(startup Startup/startup_stm32f103xb.s)
set_property(SOURCE Startup/startup_stm32f103xb.s PROPERTY LANGUAGE C)
 
# linker script
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Startup/STM32F103XB_FLASH.ld)
 
set(link_extraFlag "-lc -lm -lgcc -lnosys")
set(CMAKE_EXE_LINKER_FLAGS
"${link_extraFlag} -T${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref -Wl,--gc-sections 
-Wl,--entry=Reset_Handler -Wl,--print-memory-usage -Wl,--gc-sections"
)
 
# link directories
link_directories(
	CMSIS/inc
    CMSIS/src
    Code
    LibPer/inc
    LibPer/src
)
 
#link librarise
link_libraries(
	startup
	user_lib	
	user_code	
)
 
#generate excutable file
add_executable(${PROJECT_NAME}.elf Code/main.c)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC m)
 
#output hex and bin file
set(ELF_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf)
set(HEX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)
 
#build hex and bin file and display size
add_custom_command(TARGET "${PROJECT_NAME}.elf" POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${ELF_FILE} ${HEX_FILE}
)