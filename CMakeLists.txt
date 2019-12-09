cmake_minimum_required(VERSION 3.15)

set(AVR_UPLOADTOOL avrdude)
#set(AVR_PROGRAMMER avrispmkII)
#set(AVR_UPLOADTOOL_PORT usb)
# AVR and fuses needs to be set
set(AVR_MCU atmega32)
#set(AVR_H_FUSE 0xd9)
#set(AVR_L_FUSE 0xc3)
set(AVR_MCU_SPEED "8000000UL")


project(reflex_tester)


include_directories(${CMAKE_FIND_ROOT_PATH}/include)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE GENERATED_SOURCES ${CMAKE_SOURCE_DIR}/include/*.c ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE GENERATED_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/src/*.h)

add_avr_executable(
        ${PROJECT_NAME}
        main.c ${GENERATED_SOURCES} ${GENERATED_HEADERS}
)