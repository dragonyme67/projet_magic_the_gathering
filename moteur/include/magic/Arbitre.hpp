#pragma once

#include <string>

#include "magic/Action.hpp"
#include "magic/Etat.hpp"

namespace magic {

// Point d'insertion des règles (pour l'instant : tout est permis)
class Arbitre {
public:
    virtual ~Arbitre() = default;

    // "" = autorisé, sinon la raison du refus
    virtual std::string valider(const Etat& etat, JoueurId joueur, const Action& action) const = 0;
};

class ArbitrePermissif : public Arbitre {
public:
    std::string valider(const Etat&, JoueurId, const Action&) const override { return ""; }
};

}
