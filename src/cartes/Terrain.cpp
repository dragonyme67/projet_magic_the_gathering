#include "cartes/Terrain.hpp"

#include <utility>

Terrain::Terrain(DonneesCarte donnees, std::vector<std::string> manaProduit)
    : Permanent(std::move(donnees)), manaProduit_(std::move(manaProduit)) {}

std::string Terrain::nomType() const {
    return "Terrain";
}

bool Terrain::estDeBase() const {
    return aSupertype("Basic");
}

void Terrain::afficherDetails(std::ostream& os) const {
    if (manaProduit_.empty()) {
        return;
    }
    os << "  Mana produit :";
    for (const auto& mana : manaProduit_) {
        os << " {" << mana << '}';
    }
    os << '\n';
}
