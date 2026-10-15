# Image de développement du backend : toolchain C++, Drogon et CHR++ (chrppc).
# Le code source n'est pas copié dans l'image, docker-compose le monte en volume.
FROM debian:trixie-slim

ARG DEBIAN_FRONTEND=noninteractive
# Mêmes versions que celles installées sur la machine de développement.
ARG DROGON_VERSION=v1.9.14
ARG CHRPP_COMMIT=0f3c93669c4527e843295cf041eace665b6a563a

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake git ca-certificates pkg-config \
        libjsoncpp-dev uuid-dev zlib1g-dev libssl-dev libsqlite3-dev \
    && rm -rf /var/lib/apt/lists/*

# Drogon, avec SQLite uniquement (stockage prévu par le cahier des charges).
RUN git clone --depth 1 --branch ${DROGON_VERSION} \
        --recurse-submodules --shallow-submodules \
        https://github.com/drogonframework/drogon /tmp/drogon \
    && cmake -S /tmp/drogon -B /tmp/drogon/build \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF \
        -DBUILD_POSTGRESQL=OFF -DBUILD_MYSQL=OFF -DBUILD_REDIS=OFF \
        -DBUILD_SQLITE=ON \
    && cmake --build /tmp/drogon/build -j"$(nproc)" \
    && cmake --install /tmp/drogon/build \
    && rm -rf /tmp/drogon

# CHR++ : compilateur chrppc + en-têtes (/usr/local/bin, /usr/local/include/chrpp).
RUN git clone https://gitlab.com/vynce/chrpp.git /tmp/chrpp \
    && git -C /tmp/chrpp checkout ${CHRPP_COMMIT} \
    && cmake -S /tmp/chrpp -B /tmp/chrpp/build \
        -DCMAKE_BUILD_TYPE=Release -DEXAMPLES=OFF -DTESTS=OFF \
    && cmake --build /tmp/chrpp/build -j"$(nproc)" \
    && cmake --install /tmp/chrpp/build \
    && rm -rf /tmp/chrpp

# Utilisateur non-root (UID 1000 = premier utilisateur Linux) : les fichiers
# créés dans les volumes montés (data/app.db...) vous appartiennent.
RUN useradd --create-home --uid 1000 dev \
    && mkdir -p /app/backend/build /app/data \
    && chown -R dev:dev /app

COPY backend-entrypoint.sh /usr/local/bin/backend-entrypoint.sh
RUN chmod +x /usr/local/bin/backend-entrypoint.sh

USER dev
WORKDIR /app/backend
EXPOSE 8080
ENTRYPOINT ["backend-entrypoint.sh"]
