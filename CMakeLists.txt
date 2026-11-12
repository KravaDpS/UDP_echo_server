cmake_minimum_required(VERSION 3.22)

# Настройки компилятора
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Тип сборки по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Имя проекта
set(CMAKE_PROJECT_NAME UDP_Echo_Server)

# Генерация compile_commands.json для автодополнения (clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Инициализация проекта (C, C++, ASM)
project(${CMAKE_PROJECT_NAME} C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Создание исполняемого файла
add_executable(${CMAKE_PROJECT_NAME})

# Подключение сгенерированных скриптов STM32CubeMX
add_subdirectory(cmake/stm32cubemx)

# Удаление ошибочной зависимости libob.a при использовании C++
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Пост-билд шаги: генерация .bin, .hex и вывод размера прошивки
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMENT "Генерация .bin, .hex и отчета о размере памяти"
)