#include "cartes/Sort.hpp"

#include <utility>

Sort::Sort(DonneesCarte donnees) : Carte(std::move(donnees)) {}

bool Sort::estPermanent() const {
    return false;
}
