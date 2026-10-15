#!/bin/sh
# Compile le backend (incrémental : seuls les fichiers modifiés) puis le lance.
set -eu

BUILD_DIR=/app/backend/build

# Configuration cmake (vérification des dépendances) uniquement à la toute première
# exécution ; ensuite `cmake --build` ne la refait que si un CMakeLists.txt change.
[ -f "$BUILD_DIR/CMakeCache.txt" ] || cmake -S /app/backend -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="${BUILD_TYPE:-Debug}"
cmake --build "$BUILD_DIR" -j"$(nproc)"

# Même disposition que sur la machine hôte : le répertoire courant doit être
# backend/build pour que "../../data/app.db" (config.json) pointe vers data/.
cd "$BUILD_DIR"
exec ./backend
