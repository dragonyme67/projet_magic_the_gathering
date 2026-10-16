#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "magic/Carte.hpp"

namespace magic {

// Toutes les cartes connues, retrouvées par leur id (colonne `id` de la table Cartes).
// Le moteur ne lit aucune base : c'est l'appelant qui la remplit avec ajouter().
class Catalogue {
public:
    void ajouter(const Carte& carte);
    const Carte* trouver(const std::string& id) const;  // nullptr si l'id est inconnu
    int taille() const { return cartes_.size(); }

private:
    std::unordered_map<std::string, Carte> cartes_;
};

struct LigneDeck {
    std::string id;
    int quantite = 1;
};

// Construit un Deck à partir d'ids de cartes. Lève std::runtime_error si un id est inconnu
// ou si une quantité est inférieure à 1.
Deck construireDeck(const Catalogue& catalogue, const std::string& joueur,
                    const std::vector<LigneDeck>& lignes);

}  // namespace magic
