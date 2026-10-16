#include "Affichage.hpp"

#include <string>

using namespace magic;

namespace {

std::string etiquette(const Objet& objet, bool avecCout) {
    const Carte& carte = *objet.carte;
    std::string texte = "[" + std::to_string(objet.id) + "] " + carte.nomAffiche();
    if (avecCout && !carte.coutMana.empty()) texte += " " + carte.coutMana;
    if (!carte.force.empty() || !carte.endurance.empty()) texte += "  " + carte.force + "/" + carte.endurance;
    if (objet.engage) texte += "  (engagé)";
    return texte;
}

void afficherJoueur(std::ostream& sortie, const Etat& etat, JoueurId id) {
    const Joueur& joueur = etat.joueurs[id];
    const bool actif = id == etat.joueurActif;

    sortie << (actif ? "▶ " : "  ") << joueur.nom << " — " << joueur.vie << " PV · bibliothèque "
           << joueur.zone(Zone::Bibliotheque).size() << " · cimetière " << joueur.zone(Zone::Cimetiere).size()
           << " · exil " << joueur.zone(Zone::Exil).size() << '\n';

    const std::vector<ObjetId>& champ = joueur.zone(Zone::ChampDeBataille);
    sortie << "     champ de bataille (" << champ.size() << ")\n";
    for (ObjetId o : champ) sortie << "       " << etiquette(etat.objets[o], false) << '\n';

    const std::vector<ObjetId>& main = joueur.zone(Zone::Main);
    if (actif) {
        sortie << "     main (" << main.size() << ")\n";
        for (ObjetId o : main) sortie << "       " << etiquette(etat.objets[o], true) << '\n';
    } else {
        sortie << "     main : " << main.size() << " cartes (cachées)\n";
    }
}

}  // namespace

void afficherEtat(std::ostream& sortie, const Etat& etat) {
    sortie << "\n══════ Tour " << etat.tour << " · au tour de " << etat.joueurs[etat.joueurActif].nom
           << " ══════\n";
    for (JoueurId id = 0; id < etat.nbJoueurs(); ++id) {
        if (id != etat.joueurActif) afficherJoueur(sortie, etat, id);
    }
    afficherJoueur(sortie, etat, etat.joueurActif);
}

void afficherZone(std::ostream& sortie, const Etat& etat, JoueurId joueur, Zone zone) {
    const std::vector<ObjetId>& contenu = etat.joueurs[joueur].zone(zone);
    sortie << etat.joueurs[joueur].nom << " — " << nomZone(zone) << " (" << contenu.size() << ")\n";
    for (ObjetId o : contenu) sortie << "  " << etiquette(etat.objets[o], true) << '\n';
}

void afficherCarte(std::ostream& sortie, const Objet& objet) {
    const Carte& carte = *objet.carte;
    sortie << "[" << objet.id << "] " << carte.nomAffiche();
    if (!carte.coutMana.empty()) sortie << "  " << carte.coutMana;
    sortie << "\n  " << carte.typeAffiche();
    if (!carte.force.empty() || !carte.endurance.empty()) sortie << "  " << carte.force << "/" << carte.endurance;
    sortie << '\n';
    if (!carte.texteAffiche().empty()) sortie << "  " << carte.texteAffiche() << '\n';
}

void afficherAide(std::ostream& sortie) {
    sortie << "Les commandes s'appliquent au joueur dont c'est le tour. <id> est le numéro entre [crochets].\n"
              "  piocher                    (p)  pioche une carte\n"
              "  jouer <id>                 (j)  main → champ de bataille\n"
              "  deplacer <id> <zone>       (d)  zone : bibliotheque, main, champ, cimetiere, exil\n"
              "  engager <id>               (e)  engage une carte\n"
              "  degager <id>               (g)  dégage une carte\n"
              "  vie <n> [joueur]                gagne n PV (négatif pour en perdre) ; joueur = nom ou 1/2\n"
              "  fin                        (f)  fin du tour\n"
              "  carte <id>                 (c)  détail d'une carte\n"
              "  voir <zone> [joueur]       (v)  contenu d'une zone (cimetière, exil…)\n"
              "  journal [n]                     les n derniers événements (10 par défaut)\n"
              "  etat                            réaffiche la partie\n"
              "  aide                       (?)  cette aide\n"
              "  quitter                    (q)  quitte la partie\n"
              "Aucune règle n'est appliquée : tout est permis.\n";
}
