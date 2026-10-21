#include "Tools.hpp"

bool bibliotheque_vide(const magic::Etat& etat, int joueur) {
    return etat.joueurs[joueur].zone(magic::Zone::Bibliotheque).empty();
}

void refuser(Verdict& verdict, const std::string& regle) {
    if (verdict.regle.empty()) verdict.regle = regle;
}