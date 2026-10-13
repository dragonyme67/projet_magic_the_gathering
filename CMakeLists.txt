cmake_minimum_required(VERSION 3.16)
project(ProjetMagic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Debian/Ubuntu : sudo apt install libjsoncpp-dev
find_package(jsoncpp CONFIG REQUIRED)

# Cœur du projet, partagé par l'exécutable console, les tests et le futur serveur web.
add_library(magic_core STATIC
    src/cartes/Carte.cpp
    src/cartes/Permanent.cpp
    src/cartes/Sort.cpp
    src/cartes/Creature.cpp
    src/cartes/Artefact.cpp
    src/cartes/Enchantement.cpp
    src/cartes/Terrain.cpp
    src/cartes/Planeswalker.cpp
    src/cartes/Ephemere.cpp
    src/cartes/Rituel.cpp
    src/parser/ParserCartes.cpp
)
target_include_directories(magic_core PUBLIC src)
target_link_libraries(magic_core PUBLIC JsonCpp::JsonCpp)
target_compile_options(magic_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(magic src/main.cpp)
target_link_libraries(magic PRIVATE magic_core)
target_compile_options(magic PRIVATE -Wall -Wextra -Wpedantic)
