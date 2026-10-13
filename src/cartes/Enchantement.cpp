#include "cartes/Enchantement.hpp"

#include <utility>

Enchantement::Enchantement(DonneesCarte donnees) : Permanent(std::move(donnees)) {}

std::string Enchantement::nomType() const {
    return "Enchantement";
}

bool Enchantement::estAura() const {
    return aSousType("Aura");
}
