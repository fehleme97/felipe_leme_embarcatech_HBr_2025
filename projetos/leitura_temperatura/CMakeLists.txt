# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(leitura_temperatura C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(leitura_temperatura leitura_temperatura.c )

pico_set_program_name(leitura_temperatura "leitura_temperatura")
pico_set_program_version(leitura_temperatura "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(leitura_temperatura 0)
pico_enable_stdio_usb(leitura_temperatura 0)

# Adiciona a pasta inc ao projeto
include_directories(
    ${CMAKE_SOURCE_DIR}/inc
)

# Adiciona os arquivos da biblioteca SSD1306
target_sources(leitura_temperatura PRIVATE
    ${CMAKE_SOURCE_DIR}/inc/ssd1306_i2c.c
)

# Add the standard library to the build
target_link_libraries(leitura_temperatura
    pico_stdlib
    hardware_i2c
    hardware_gpio
    hardware_adc
    hardware_clocks
        )

# Add the standard include files to the build
target_include_directories(leitura_temperatura PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(leitura_temperatura 
        
        )

pico_add_extra_outputs(leitura_temperatura)

