cmake_minimum_required(VERSION 3.16)
project(moteur CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Moteur seul : sans Drogon, SQLite ni jsoncpp
add_library(magic_core STATIC
    src/Catalogue.cpp
    src/Etat.cpp
    src/Game.cpp
    src/Partie.cpp
)
target_include_directories(magic_core PUBLIC include)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(magic_core PRIVATE -Wall -Wextra)
endif()

# Console (optionnelle : demande sqlite3 et jsoncpp)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(SQLITE3 IMPORTED_TARGET sqlite3)
    pkg_check_modules(JSONCPP IMPORTED_TARGET jsoncpp)
endif()
if(SQLITE3_FOUND AND JSONCPP_FOUND)
    add_executable(console_moteur
        console/main.cpp
        console/Affichage.cpp
        console/Chargeurs.cpp
        console/Console.cpp
    )
    target_link_libraries(console_moteur PRIVATE magic_core PkgConfig::SQLITE3 PkgConfig::JSONCPP)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(console_moteur PRIVATE -Wall -Wextra)
    endif()
else()
    message(STATUS "console_moteur ignorée : sqlite3 ou jsoncpp introuvable (pkg-config)")
endif()
