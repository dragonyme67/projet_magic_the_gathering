#pragma once

#include <string>

#include "magic/Action.hpp"
#include "magic/Etat.hpp"

namespace magic {

// Point d'insertion des règles du jeu : Partie demande à l'arbitre avant d'exécuter une action.
// Aujourd'hui tout est permis (ArbitrePermissif). Plus tard, un arbitre basé sur les règles CHR++.
class Arbitre {
public:
    virtual ~Arbitre() = default;

    // Renvoie la raison du refus, ou une chaîne vide si l'action est autorisée.
    virtual std::string valider(const Etat& etat, JoueurId joueur, const Action& action) const = 0;
};

class ArbitrePermissif : public Arbitre {
public:
    std::string valider(const Etat&, JoueurId, const Action&) const override { return ""; }
};

}  // namespace magic
