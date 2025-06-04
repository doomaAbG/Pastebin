# Указываем минимальную версию CMake
cmake_minimum_required(VERSION 3.20)

# Название проекта и язык (C++)
project(pastebin LANGUAGES CXX)

# Устанавливаем стандарт C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Указываем путь к Asio
set(ASIO_ROOT "C:/Asio/asio-1.30.2")
include_directories(${ASIO_ROOT}/include)

# Подключаем libpqxx через vcpkg
set(PQXX_ROOT "C:/Users/beenk/vcpkg/installed/x64-windows-static")
include_directories(${PQXX_ROOT}/include)
link_directories(${PQXX_ROOT}/lib)
add_definitions(-DPQXX_STATIC)

# Указываем путь к Crow
include_directories("C:/Crow/Crow/include")

# Создаём исполняемый файл pastebin из main.cpp
add_executable(pastebin src/main.cpp)

# Подключаем библиотеки
target_link_libraries(pastebin PRIVATE pqxx)

# Определяем _WIN32_WINNT для Asio (Windows 10)
add_definitions(-D_WIN32_WINNT=0x0A00)

# Добавляем поддержку UTF-8 для устранения предупреждений C4566
add_compile_options(/utf-8)