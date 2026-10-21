#pragma once

#include <memory>
#include <string>

#include "magic/Arbitre.hpp"

namespace magic {

class ArbitreChr : public Arbitre {
public:
    std::string valider(const Etat& etat, JoueurId joueur, const Action& action) const override;
};

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}
