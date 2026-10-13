#pragma once

#include "cartes/Sort.hpp"

// Sort lançable à tout moment où le joueur a la priorité.
class Ephemere : public Sort {
public:
    explicit Ephemere(DonneesCarte donnees);

    std::string nomType() const override;
};
