#Khi tạo project mới, thay đổi {tên project} ở dòng 7: project({tên project} C ASM)
#Thêm file .c ở mục 1.
#nếu  file gcc-arm-none-eabi.cmake ko nằm cùng cấp với file này (vd nằm trong thư mục cmake) thì chỉnh đường dẫn ở dòng 10


cmake_minimum_required(VERSION 3.16)
project(projectw4 C ASM)

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/gcc-arm-none-eabi.cmake")
endif()

# Macro bat buoc cho dong chip STM32F103 va thu vien HAL
add_compile_definitions(STM32F103xB USE_HAL_DRIVER)

# Khai bao file thuc thi elf
add_executable(${PROJECT_NAME})

# 1. Dua cac file nguon vao muc bien dich (Giu folder Drivers de tai su dung)
target_sources(${PROJECT_NAME} PRIVATE
    main.c
    system_stm32f1xx.c
    startup_stm32f103xb.s
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
)

# 2. Khai bao cac thu muc chua file tieu de (.h)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    Drivers/STM32F1xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32F1xx/Include
    Drivers/CMSIS/Include
)

# Xuat truc tiep file .hex va .bin vao thu muc build hien tai ma Ninja dang dung
set(HEX_FILE ${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
    COMMENT "Da build xong! File hex va bin nam trong thu muc build/"
)




