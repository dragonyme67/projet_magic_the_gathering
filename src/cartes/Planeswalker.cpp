#include "cartes/Planeswalker.hpp"

#include <utility>

Planeswalker::Planeswalker(DonneesCarte donnees, int loyaute)
    : Permanent(std::move(donnees)), loyaute_(loyaute) {}

std::string Planeswalker::nomType() const {
    return "Planeswalker";
}

void Planeswalker::afficherDetails(std::ostream& os) const {
    os << "  Loyauté : " << loyaute_ << '\n';
}
