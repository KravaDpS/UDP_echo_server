cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

# Макросы для STM32F429
set(MX_Defines_Syms 
    USE_HAL_DRIVER 
    STM32F429xx
    $<$<CONFIG:Debug>:DEBUG>
)

# Пути к заголовочным файлам (ДОБАВЛЕН ПУТЬ К BSP)
set(MX_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/BSP/STM32F4xx_Nucleo
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/LwIP/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/LwIP/src/include/ipv4
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/LwIP/src/include/ipv6
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/LwIP/system
)

# Исходные файлы приложения (ДОБАВЛЕН ФАЙЛ BSP)
set(MX_Application_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/stm32f4xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/system_stm32f4xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/app_ethernet.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/ethernetif.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/hw_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/udp_server.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/utilities.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Startup/startup_stm32f429zitx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/BSP/STM32F4xx_Nucleo/stm32f4xx_nucleo_144.c
)

# Автоматический поиск всех .c файлов HAL-драйверов STM32F4
file(GLOB_RECURSE STM32_Drivers_Src 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32F4xx_HAL_Driver/Src/*.c
)

# Автоматический поиск всех .c файлов LwIP
file(GLOB_RECURSE LWIP_Src 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/LwIP/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Middlewares/Third_Party/LwIP/system/*.c
)

# Интерфейсная библиотека для макросов и инклюдов
add_library(stm32cubemx INTERFACE)
target_include_directories(stm32cubemx INTERFACE ${MX_Include_Dirs})
target_compile_definitions(stm32cubemx INTERFACE ${MX_Defines_Syms})

# Статическая библиотека драйверов
add_library(STM32_Drivers OBJECT)
target_sources(STM32_Drivers PRIVATE ${STM32_Drivers_Src})
target_link_libraries(STM32_Drivers PUBLIC stm32cubemx)

# Статическая библиотека LwIP
add_library(LwIP_Library OBJECT)
target_sources(LwIP_Library PRIVATE ${LWIP_Src})
target_link_libraries(LwIP_Library PUBLIC stm32cubemx)

# Добавляем исходники приложения к целевому исполняемому файлу
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MX_Application_Src})

# Линкуем библиотеки
target_link_libraries(${CMAKE_PROJECT_NAME} 
    STM32_Drivers
    LwIP_Library
    ${TOOLCHAIN_LINK_LIBRARIES}
)

# Очистка .map файла при команде clean
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Проверка стандарта C
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(FATAL_ERROR "Generated code requires C11 or higher")
endif()