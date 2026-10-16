#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "magic/Carte.hpp"

namespace magic {


class Catalogue {
public:
    void ajouter(Carte carte);                           
    const Carte* trouver(const std::string& id) const; 
    std::size_t taille() const { return cartes_.size(); }

private:
    std::unordered_map<std::string, Carte> cartes_;
};

struct LigneDeck {
    std::string id;
    int quantite = 1;
};

Deck construireDeck(const Catalogue& catalogue, const std::string& joueur,
                    const std::vector<LigneDeck>& lignes);

}  // namespace magic
