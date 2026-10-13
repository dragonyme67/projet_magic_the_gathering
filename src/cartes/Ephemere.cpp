#include "cartes/Ephemere.hpp"

#include <utility>

Ephemere::Ephemere(DonneesCarte donnees) : Sort(std::move(donnees)) {}

std::string Ephemere::nomType() const {
    return "Éphémère";
}
