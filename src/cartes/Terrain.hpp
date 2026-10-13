#pragma once

#include "cartes/Permanent.hpp"

class Terrain : public Permanent {
public:
    Terrain(DonneesCarte donnees, std::vector<std::string> manaProduit);

    std::string nomType() const override;

    bool estDeBase() const;
    // Couleurs de mana que le terrain peut produire (W, U, B, R, G, C).
    const std::vector<std::string>& manaProduit() const { return manaProduit_; }

protected:
    void afficherDetails(std::ostream& os) const override;

private:
    std::vector<std::string> manaProduit_;
};
