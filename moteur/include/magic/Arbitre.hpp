#pragma once

#include <optional>
#include <string>

#include "magic/Action.hpp"
#include "magic/Etat.hpp"

namespace magic {


class Arbitre {
public:
    virtual ~Arbitre() = default;

    // Renvoie la raison du refus, ou std::nullopt si l'action est autorisée.
    virtual std::optional<std::string> valider(const Etat& etat, JoueurId joueur,
                                               const Action& action) const = 0;
};

class ArbitrePermissif : public Arbitre {
public:
    std::optional<std::string> valider(const Etat&, JoueurId, const Action&) const override {
        return std::nullopt;
    }
};

}  // namespace magic
