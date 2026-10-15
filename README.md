# Projet Magic

Installer :
sudo apt install libjsoncpp-dev

Lancer avec Docker (frontend sur :5173, backend sur :8080) :

    docker compose up -d
    docker compose down

Docker n'installe rien : il utilise la toolchain de la machine (g++, cmake, node, Drogon,
chrppc, `frontend/node_modules`, `backend/build`). Prérequis : machine Debian 13, dépendances
déjà installées, commande lancée depuis la racine du projet.