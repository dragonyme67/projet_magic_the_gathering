#pragma once

#include <string>

#include "magic/Etat.hpp"

struct Verdict {
    std::string regle;
};

bool bibliotheque_vide(const magic::Etat& etat, int joueur);
void refuser(Verdict& verdict, const std::string& regle);