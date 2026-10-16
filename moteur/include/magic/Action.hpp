#pragma once

#include <variant>

#include "magic/Etat.hpp"

namespace magic {

// Ce qu'un joueur peut demander. Une action = une intention ; son effet est décrit
// dans Partie.cpp. Pour en ajouter une : une struct ici, l'ajouter à `Action`, puis la
// surcharge `Partie::executer` correspondante (un oubli ne passe pas inaperçu :
// le `std::visit` de Partie::appliquer ne compile pas sans elle).
struct Piocher {};                                  // le joueur qui agit pioche
struct Deplacer { ObjetId objet; Zone destination; };
struct Engager { ObjetId objet; bool engage; };
struct ChangerVie { JoueurId cible; int delta; };
struct FinDuTour {};

using Action = std::variant<Piocher, Deplacer, Engager, ChangerVie, FinDuTour>;

}  // namespace magic
