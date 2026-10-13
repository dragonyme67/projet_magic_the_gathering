#include "cartes/Rituel.hpp"

#include <utility>

Rituel::Rituel(DonneesCarte donnees) : Sort(std::move(donnees)) {}

std::string Rituel::nomType() const {
    return "Rituel";
}
