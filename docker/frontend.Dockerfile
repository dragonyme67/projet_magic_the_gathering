# Image de développement du frontend (Vite). Le code est monté par docker-compose.
# Contexte de build : ./frontend (voir docker-compose.yml).
FROM node:22-slim

# Droits pour l'utilisateur node (UID 1000) : le volume node_modules les hérite.
RUN mkdir -p /app/frontend && chown -R node:node /app

USER node
WORKDIR /app/frontend

# Dépendances installées une seule fois, ici, au build de l'image.
COPY --chown=node:node package.json package-lock.json ./
RUN npm ci

EXPOSE 5173
