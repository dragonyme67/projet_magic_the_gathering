#pragma once

#include "cartes/Permanent.hpp"

class Planeswalker : public Permanent {
public:
    Planeswalker(DonneesCarte donnees, int loyaute);

    std::string nomType() const override;

    int loyaute() const { return loyaute_; }

protected:
    void afficherDetails(std::ostream& os) const override;

private:
    int loyaute_;
};
