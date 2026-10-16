#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "magic/Carte.hpp"

namespace magic {

// Cartes par id, remplies par l'appelant (le moteur ne lit aucune base)
class Catalogue {
public:
    void ajouter(const Carte& carte);
    const Carte* trouver(const std::string& id) const;  // nullptr si inconnu
    int taille() const { return cartes_.size(); }

private:
    std::unordered_map<std::string, Carte> cartes_;
};

struct LigneDeck {
    std::string id;
    int quantite = 1;
};

// Lève std::runtime_error si id inconnu ou quantité < 1
Deck construireDeck(const Catalogue& catalogue, const std::string& joueur,
                    const std::vector<LigneDeck>& lignes);

}
