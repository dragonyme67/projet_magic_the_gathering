cmake_minimum_required(VERSION 3.16)
project(moteur CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra)

# chrppc transforme regles/Regles.chrpp en C++
set(DOSSIER_CHR ${CMAKE_CURRENT_BINARY_DIR}/chr)
add_custom_command(
    OUTPUT ${DOSSIER_CHR}/ReglesARBITRE.cpp ${DOSSIER_CHR}/ReglesARBITRE.hh ${DOSSIER_CHR}/Regles_chr.cpp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOSSIER_CHR}
    COMMAND chrppc --output_dir ${DOSSIER_CHR} ${CMAKE_CURRENT_SOURCE_DIR}/regles/Regles.chrpp
    DEPENDS regles/Regles.chrpp regles/Tools.hpp
)

# Le moteur : règles de jeu et arbitre
add_library(magic_core STATIC
    src/ArbitreChr.cpp
    src/Etat.cpp
    src/Game.cpp
    src/Partie.cpp
    regles/Tools.cpp
    ${DOSSIER_CHR}/ReglesARBITRE.cpp
    ${DOSSIER_CHR}/Regles_chr.cpp
)
target_include_directories(magic_core PUBLIC include PRIVATE regles ${DOSSIER_CHR} /usr/local/include/chrpp)

# Chargement des decks (SQLite + JSON)
add_library(magic_io STATIC io/src/DeckRepository.cpp)
target_include_directories(magic_io PUBLIC io/include /usr/include/jsoncpp)
target_link_libraries(magic_io PUBLIC magic_core sqlite3 jsoncpp)

# Jeu en console
add_executable(console_moteur
    console/main.cpp
    console/Affichage.cpp
    console/Console.cpp
)
target_link_libraries(console_moteur PRIVATE magic_io)
