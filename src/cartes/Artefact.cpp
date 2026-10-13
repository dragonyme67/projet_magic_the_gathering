#include "cartes/Artefact.hpp"

#include <utility>

Artefact::Artefact(DonneesCarte donnees, std::optional<ForceEndurance> forceEndurance)
    : Permanent(std::move(donnees)), forceEndurance_(forceEndurance) {}

std::string Artefact::nomType() const {
    return "Artefact";
}

bool Artefact::estVehicule() const {
    return aSousType("Vehicle");
}

void Artefact::afficherDetails(std::ostream& os) const {
    if (forceEndurance_) {
        os << "  Force/Endurance : " << forceEndurance_->force << '/' << forceEndurance_->endurance
           << (estVehicule() ? " (véhicule)" : "") << '\n';
    }
}
