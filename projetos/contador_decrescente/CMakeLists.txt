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
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Nome do projeto
project(contador_decrescente C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Adiciona o executável principal
add_executable(contador_decrescente contador_decrescente.c inc/ssd1306_i2c.c)

pico_set_program_name(contador_decrescente "BitDogLab_contador_decrescente")
pico_set_program_version(contador_decrescente "0.1")

# Habilita a saída de printf via USB (se necessário)
pico_enable_stdio_usb(contador_decrescente 1)
pico_enable_stdio_uart(contador_decrescente 0)

# Adiciona a pasta inc ao projeto
include_directories(
    ${CMAKE_SOURCE_DIR}/inc
)

# Adiciona os arquivos da biblioteca SSD1306
target_sources(contador_decrescente PRIVATE
    ${CMAKE_SOURCE_DIR}/inc/ssd1306_i2c.c
)

# Linka as bibliotecas necessárias
target_link_libraries(contador_decrescente
    pico_stdlib
    hardware_i2c
    hardware_gpio
    hardware_adc
    hardware_clocks
)

# Add the standard include files to the build
target_include_directories(contador_decrescente PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Configurações adicionais para otimização (opcional)
target_compile_options(contador_decrescente PRIVATE -O2)

# Adiciona o arquivo binário .uf2 para fácil upload
pico_add_extra_outputs(contador_decrescente)