# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_BUILD_TYPE "Release") 

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/home/jabba/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(pico-DCO-test C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico-DCO-test)

pico_generate_pio_header(pico-DCO-test ${CMAKE_CURRENT_LIST_DIR}/piodco/dco.pio)

target_sources(pico-DCO-test PUBLIC
	      ${CMAKE_CURRENT_LIST_DIR}/lib/assert.c
        ${CMAKE_CURRENT_LIST_DIR}/piodco/piodco.c
        ${CMAKE_CURRENT_LIST_DIR}/test.c
        )

pico_set_program_name(pico-DCO-test "pico-DCO-test")
pico_set_program_version(pico-DCO-test "0.9")

pico_enable_stdio_uart(pico-DCO-test 1)
pico_enable_stdio_usb(pico-DCO-test 0)

# Add the standard include files to the build
target_include_directories(pico-DCO-test PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(
        pico-DCO-test
        pico_stdlib
        pico_sync
        pico_multicore
        hardware_timer
        hardware_clocks
        hardware_pio
        hardware_vreg
        )

pico_add_extra_outputs(pico-DCO-test)
