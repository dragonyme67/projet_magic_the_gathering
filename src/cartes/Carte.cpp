#include "cartes/Carte.hpp"

#include <algorithm>
#include <sstream>
#include <utility>

namespace {

bool contient(const std::vector<std::string>& liste, const std::string& valeur) {
    return std::find(liste.begin(), liste.end(), valeur) != liste.end();
}

}  // namespace

Carte::Carte(DonneesCarte donnees) : donnees_(std::move(donnees)) {}

bool Carte::aSupertype(const std::string& supertype) const {
    return contient(donnees_.supertypes, supertype);
}

bool Carte::aType(const std::string& type) const {
    return contient(donnees_.types, type);
}

bool Carte::aSousType(const std::string& sousType) const {
    return contient(donnees_.sousTypes, sousType);
}

bool Carte::estLegendaire() const {
    return aSupertype("Legendary");
}

void Carte::afficher(std::ostream& os) const {
    os << nomFr() << " (" << nom() << ")";
    if (!coutMana().empty()) {
        os << "  " << coutMana();
    }
    os << "\n  " << ligneTypeFr() << "  [" << nomType() << ", "
       << (estPermanent() ? "permanent" : "sort") << "]\n";

    afficherDetails(os);

    // Le texte peut tenir sur plusieurs lignes : on les indente toutes.
    std::istringstream lignes(texteFr().empty() ? texte() : texteFr());
    for (std::string ligne; std::getline(lignes, ligne);) {
        os << "  " << ligne << '\n';
    }
}

void Carte::afficherDetails(std::ostream&) const {}

std::ostream& operator<<(std::ostream& os, const Carte& carte) {
    carte.afficher(os);
    return os;
}
