#pragma once

#include <string>
#include <vector>

#include "magic/Game.hpp"

// Jeu en console : lit les commandes tapées au clavier, les transforme en Action et les
// envoie à Game. Les deux joueurs jouent sur le même clavier ; une commande s'applique
// toujours au joueur dont c'est le tour.
class Console {
public:
    explicit Console(magic::Game& jeu);

    // Lance une partie si aucune n'est en cours, puis lit les commandes jusqu'à « quitter ».
    void run();

private:
    // Exécute une commande. Renvoie faux pour quitter.
    bool traiter(const std::string& commande, const std::vector<std::string>& args);

    // Une fonction par commande.
    void jouer(const std::vector<std::string>& args);
    void deplacer(const std::vector<std::string>& args);
    void engager(const std::vector<std::string>& args, bool engage);
    void vie(const std::vector<std::string>& args);
    void carte(const std::vector<std::string>& args);
    void voir(const std::vector<std::string>& args);
    void journal(const std::vector<std::string>& args);

    // Envoie l'action au jeu pour le joueur actif et affiche ce qui s'est passé.
    void appliquer(const magic::Action& action);

    // Vrai s'il y a assez d'arguments ; sinon affiche l'usage.
    bool argumentsSuffisants(const std::vector<std::string>& args, int minimum, const char* usage) const;
    // Lit l'id d'une carte tapé par le joueur ; affiche un message et renvoie faux s'il est invalide.
    bool lireObjet(const std::string& texte, magic::ObjetId& id) const;

    magic::Game& jeu_;
};
