#pragma once

#include <string>

#include "magic/Arbitre.hpp"

namespace magic {

class ArbitreChr : public Arbitre {
public:
    std::string valider(const Etat& etat, JoueurId joueur, const Action& action) const override;
};

}
