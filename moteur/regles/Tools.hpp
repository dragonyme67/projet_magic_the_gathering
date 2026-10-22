#pragma once

#include <string>

#include "magic/Etat.hpp"

struct Verdict {
    std::string regle;
};

void refuser(Verdict& verdict, const std::string& regle);
bool carte_engagee(const magic::Etat& etat, int objet);
bool bibliotheque_vide(const magic::Etat& etat, int joueur);