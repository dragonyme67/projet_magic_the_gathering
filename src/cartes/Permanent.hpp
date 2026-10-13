#pragma once

#include "cartes/Carte.hpp"

// Carte qui reste sur le champ de bataille : créature, artefact, enchantement, terrain, planeswalker.
class Permanent : public Carte {
public:
    explicit Permanent(DonneesCarte donnees);

    bool estPermanent() const override;
};
