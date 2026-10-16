#pragma once

#include <string>

namespace magic {

// Ligne du journal (base des explications)
struct Evenement {
    int id = 0;
    int cause = -1;  // id de l'événement cause, -1 = action du joueur
    std::string regle;
    std::string texte;
};

}
