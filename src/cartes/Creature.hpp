#pragma once

#include "cartes/Permanent.hpp"

class Creature : public Permanent {
public:
    Creature(DonneesCarte donnees, ForceEndurance forceEndurance);

    std::string nomType() const override;

    int force() const { return forceEndurance_.force; }
    int endurance() const { return forceEndurance_.endurance; }

protected:
    void afficherDetails(std::ostream& os) const override;

private:
    ForceEndurance forceEndurance_;
};
