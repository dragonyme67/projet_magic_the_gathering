#pragma once

#include "cartes/Carte.hpp"

// Carte qui produit son effet puis va au cimetière : éphémère, rituel.
class Sort : public Carte {
public:
    explicit Sort(DonneesCarte donnees);

    bool estPermanent() const override;
};
