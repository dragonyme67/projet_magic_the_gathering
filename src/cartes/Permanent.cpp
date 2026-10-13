#include "cartes/Permanent.hpp"

#include <utility>

Permanent::Permanent(DonneesCarte donnees) : Carte(std::move(donnees)) {}

bool Permanent::estPermanent() const {
    return true;
}
