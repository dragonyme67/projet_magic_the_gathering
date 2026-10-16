#pragma once

#include <string>

namespace magic {

// Une ligne du journal de partie. C'est la base du futur mécanisme d'explication :
// chaque règle qui s'appliquera produira un événement lié à sa cause.
struct Evenement {
    int id = 0;
    int cause = -1;      // id de l'événement qui l'a provoqué ; -1 = action directe du joueur
    std::string regle;   // identifiant de la règle (pour l'instant : le nom de l'action)
    std::string texte;   // phrase affichable
};

}  // namespace magic
