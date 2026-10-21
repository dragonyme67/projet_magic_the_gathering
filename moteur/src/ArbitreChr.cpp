#include "magic/ArbitreChr.hpp"

#include "Tools.hpp"
#include "ReglesARBITRE.hh"

namespace magic {

namespace {

std::string translate(const std::string& regle) {
    if (regle == "pas_son_tour") return "Ce n'est pas votre tour.";
    if (regle == "piocher_vide") return "La bibliothèque est vide.";
    return "Action refusée (règle " + regle + ").";
}

}

std::string ArbitreChr::valider(const Etat& etat, JoueurId joueur, const Action& action) const {
    Verdict verdict;
    auto espace = ARBITRE::create(etat, verdict);
    espace->demande(static_cast<int>(joueur), static_cast<int>(action.type), etat.tour);

    if (verdict.regle.empty()) return "";
    return translate(verdict.regle);
}

}
