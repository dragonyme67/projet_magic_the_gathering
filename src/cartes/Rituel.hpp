#pragma once

#include "cartes/Sort.hpp"

// Sort lançable uniquement pendant sa phase principale, pile vide.
class Rituel : public Sort {
public:
    explicit Rituel(DonneesCarte donnees);

    std::string nomType() const override;
};
