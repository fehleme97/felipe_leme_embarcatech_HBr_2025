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
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configura ambiente SDK
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)

project(teste_unitario_adc C CXX ASM)

pico_sdk_init()

# =============================
# LIB: Unity
# =============================
add_library(Unity STATIC
    Unity/src/unity.c
)
target_include_directories(Unity PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Unity/src
)

# =============================
# LIB: módulo de temperatura
# =============================
add_library(adc_temp STATIC
    module_a/adc_temp.c
)
target_include_directories(adc_temp PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/module_a
)

# =============================
# APP: Aplicação principal
# =============================

# Comentado temporariamente para evitar erro de linkagem
# add_executable(teste_unitario_adc
#     teste_unitario_adc/teste_unitario_adc.c
# )
# target_link_libraries(teste_unitario_adc
#     pico_stdlib
#     adc_temp
# )
# target_include_directories(teste_unitario_adc PRIVATE
#     ${CMAKE_CURRENT_LIST_DIR}/module_a
# )
# pico_enable_stdio_uart(teste_unitario_adc 0)
# pico_enable_stdio_usb(teste_unitario_adc 1)
# pico_add_extra_outputs(teste_unitario_adc)


# =============================
# TEST: Executável de Testes
# =============================
add_executable(test_runner
    test/test_adc_temp.c
    module_a/adc_temp.c
    module_a/adc_temp.h
)
target_link_libraries(test_runner
    pico_stdlib
    adc_temp
    Unity
)
target_include_directories(test_runner PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/module_a
    ${CMAKE_CURRENT_LIST_DIR}/test
    ${CMAKE_CURRENT_LIST_DIR}/Unity/src
)

pico_enable_stdio_uart(test_runner 0)
pico_enable_stdio_usb(test_runner 1)

pico_add_extra_outputs(test_runner)
