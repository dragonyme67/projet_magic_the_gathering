# Projet Magic

Installer :
sudo apt install libjsoncpp-dev

Lancer avec Docker (frontend sur :5173, backend sur :8080) :

    docker compose up -d
    docker compose down

Lancer moteur jeu :
    
    cmake -S moteur -B moteur/build && cmake --build moteur/build
    ./moteur/build/console_moteur