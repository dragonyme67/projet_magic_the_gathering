#pragma once

#include "cartes/Permanent.hpp"

class Enchantement : public Permanent {
public:
    explicit Enchantement(DonneesCarte donnees);

    std::string nomType() const override;

    // Une aura s'attache à un permanent ou un joueur.
    bool estAura() const;
};
