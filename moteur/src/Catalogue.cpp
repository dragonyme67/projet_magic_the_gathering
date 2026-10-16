#include "magic/Catalogue.hpp"

#include <stdexcept>
#include <utility>

namespace magic {

void Catalogue::ajouter(Carte carte) {
    std::string id = carte.id;
    cartes_.insert_or_assign(std::move(id), std::move(carte));
}

const Carte* Catalogue::trouver(const std::string& id) const {
    const auto it = cartes_.find(id);
    return it == cartes_.end() ? nullptr : &it->second;
}

Deck construireDeck(const Catalogue& catalogue, const std::string& joueur,
                    const std::vector<LigneDeck>& lignes) {
    Deck deck{joueur, {}};
    for (const LigneDeck& ligne : lignes) {
        const Carte* carte = catalogue.trouver(ligne.id);
        if (!carte) {
            throw std::runtime_error("Deck de " + joueur + " : carte inconnue (id " + ligne.id + ").");
        }
        if (ligne.quantite < 1) {
            throw std::runtime_error("Deck de " + joueur + " : quantité invalide pour " + ligne.id + ".");
        }
        deck.cartes.insert(deck.cartes.end(), static_cast<std::size_t>(ligne.quantite), *carte);
    }
    return deck;
}

}  // namespace magic
