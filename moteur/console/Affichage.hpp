#pragma once

#include <ostream>

#include "magic/Etat.hpp"

// adversaire d'abord, joueur actif (avec sa main) en dernier
void afficherEtat(std::ostream& sortie, const magic::Etat& etat);

void afficherZone(std::ostream& sortie, const magic::Etat& etat, magic::JoueurId joueur, magic::Zone zone);

void afficherCarte(std::ostream& sortie, const magic::Objet& objet);

void afficherAide(std::ostream& sortie);
