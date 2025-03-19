cmake_minimum_required(VERSION 3.28)
project(Projekt)

set(CMAKE_CXX_STANDARD 26)

# Flaga modułów dla modułów
#add_compile_options(-fmodules-ts)

add_executable(Projekt main.cpp
        interfejs.h
        interfejs.cpp
        Gracz.h
        Gracz.cpp
        Global.h)

# Katalogi nagłówków
#include_directories(${CMAKE_SOURCE_DIR})

# Ścieżka do SFML
set(SFML_DIR "C:/Users/kacpe/OneDrive/Studia/PK4/SFML-2.6.1/lib/cmake/SFML")

# Znajdź pakiet SFML
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(Projekt sfml-system sfml-window sfml-graphics sfml-network sfml-audio)