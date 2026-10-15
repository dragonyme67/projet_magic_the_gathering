#!/bin/sh
# Compile le backend (incrémental : seuls les fichiers modifiés) puis le lance.
# Lancé depuis <projet>/backend (working_dir de docker-compose.yml), avec la
# toolchain de l'hôte : rien n'est installé ni reconfiguré si le cache CMake existe.
set -eu

SRC_DIR=$(pwd)
BUILD_DIR="$SRC_DIR/build"

# Configuration cmake uniquement si backend/build n'existe pas encore ; ensuite
# `cmake --build` ne la refait que si un CMakeLists.txt change.
[ -f "$BUILD_DIR/CMakeCache.txt" ] || cmake -S "$SRC_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="${BUILD_TYPE:-Debug}"
cmake --build "$BUILD_DIR" -j"$(nproc)"

# Le répertoire courant reste backend/ : les chemins relatifs de config.json
# ("../data/app.db", "uploads", document_root "./") en dépendent.
exec "$BUILD_DIR/backend"
