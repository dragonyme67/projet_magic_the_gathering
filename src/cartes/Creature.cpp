#include "cartes/Creature.hpp"

#include <utility>

Creature::Creature(DonneesCarte donnees, ForceEndurance forceEndurance)
    : Permanent(std::move(donnees)), forceEndurance_(forceEndurance) {}

std::string Creature::nomType() const {
    return "Créature";
}

void Creature::afficherDetails(std::ostream& os) const {
    os << "  Force/Endurance : " << force() << '/' << endurance() << '\n';
}
