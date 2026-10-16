#pragma once

#include <ostream>

#include "magic/Etat.hpp"

// Tout ce que la console écrit sur l'état de la partie. Aucune logique de jeu ici.

// Tour, puis chaque joueur : l'adversaire d'abord, le joueur actif en dernier avec sa main.
void afficherEtat(std::ostream& sortie, const magic::Etat& etat);

// Contenu d'une zone, un objet par ligne.
void afficherZone(std::ostream& sortie, const magic::Etat& etat, magic::JoueurId joueur, magic::Zone zone);

// Détail d'une carte (coût, type, force/endurance, texte).
void afficherCarte(std::ostream& sortie, const magic::Objet& objet);

void afficherAide(std::ostream& sortie);
