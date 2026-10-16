#pragma once

#include <string>
#include <vector>

#include "magic/Game.hpp"

// Jeu en console : deux joueurs au même clavier, commandes appliquées au joueur du tour
class Console {
public:
    explicit Console(magic::Game& jeu);

    void run();

private:
    // faux = quitter
    bool traiter(const std::string& commande, const std::vector<std::string>& args);

    void jouer(const std::vector<std::string>& args);
    void deplacer(const std::vector<std::string>& args);
    void engager(const std::vector<std::string>& args, bool engage);
    void vie(const std::vector<std::string>& args);
    void carte(const std::vector<std::string>& args);
    void voir(const std::vector<std::string>& args);
    void journal(const std::vector<std::string>& args);

    void appliquer(const magic::Action& action);

    // affiche l'usage s'il manque des arguments
    bool argumentsSuffisants(const std::vector<std::string>& args, int minimum, const char* usage) const;
    // affiche l'erreur si l'id est invalide
    bool lireObjet(const std::string& texte, magic::ObjetId& id) const;

    magic::Game& jeu_;
};
