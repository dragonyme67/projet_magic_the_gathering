#include "magic/Catalogue.hpp"

#include <stdexcept>

namespace magic {

void Catalogue::ajouter(const Carte& carte) {
    cartes_[carte.id] = carte;  // remplace la carte si l'id existe déjà
}

const Carte* Catalogue::trouver(const std::string& id) const {
    std::unordered_map<std::string, Carte>::const_iterator it = cartes_.find(id);
    if (it == cartes_.end()) return nullptr;
    return &it->second;
}

Deck construireDeck(const Catalogue& catalogue, const std::string& joueur,
                    const std::vector<LigneDeck>& lignes) {
    Deck deck;
    deck.joueur = joueur;

    for (const LigneDeck& ligne : lignes) {
        const Carte* carte = catalogue.trouver(ligne.id);
        if (carte == nullptr) {
            throw std::runtime_error("Deck de " + joueur + " : carte inconnue (id " + ligne.id + ").");
        }
        if (ligne.quantite < 1) {
            throw std::runtime_error("Deck de " + joueur + " : quantité invalide pour " + ligne.id + ".");
        }
        for (int i = 0; i < ligne.quantite; ++i) {
            deck.cartes.push_back(*carte);
        }
    }
    return deck;
}

}  // namespace magic
