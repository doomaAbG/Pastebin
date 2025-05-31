# Указываем минимальную версию CMake
cmake_minimum_required(VERSION 3.20)

# Название проекта и язык (C++)
project(pastebin LANGUAGES CXX)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Указываем путь к Asio
set(ASIO_ROOT "C:/Asio/asio-1.30.2")
include_directories(${ASIO_ROOT}/include)

# Создаём исполняемый файл pastebin из main.cpp
add_executable(pastebin src/main.cpp)

# Указываем путь к Crow
target_include_directories(pastebin PRIVATE "C:/Crow/Crow/include")

# Определяем _WIN32_WINNT для Asio (Windows 10)
add_definitions(-D_WIN32_WINNT=0x0A00)