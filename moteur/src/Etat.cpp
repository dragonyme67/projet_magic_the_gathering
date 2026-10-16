#include "magic/Etat.hpp"

#include <utility>

namespace magic {

const char* nomZone(Zone zone) {
    switch (zone) {
        case Zone::Bibliotheque:    return "bibliothèque";
        case Zone::Main:            return "main";
        case Zone::ChampDeBataille: return "champ de bataille";
        case Zone::Cimetiere:       return "cimetière";
        case Zone::Exil:            return "exil";
        case Zone::Nombre:          break;
    }
    return "?";
}

ObjetId Etat::creerObjet(std::shared_ptr<const Carte> carte, JoueurId proprietaire, Zone zone) {
    Joueur& joueur = joueurs.at(proprietaire);

    Objet objet;
    objet.id = nbObjets();
    objet.carte = std::move(carte);
    objet.proprietaire = proprietaire;
    objet.zone = zone;
    objets.push_back(objet);

    joueur.zone(zone).push_back(objet.id);
    return objet.id;
}

void Etat::deplacer(ObjetId id, Zone destination) {
    Objet& objet = objets.at(id);
    Joueur& proprietaire = joueurs.at(objet.proprietaire);

    std::vector<ObjetId>& origine = proprietaire.zone(objet.zone);
    const int taille = origine.size();
    for (int i = 0; i < taille; ++i) {
        if (origine[i] == id) {
            origine.erase(origine.begin() + i);
            break;
        }
    }

    proprietaire.zone(destination).push_back(id);
    objet.zone = destination;
    objet.engage = false;  // changer de zone dégage
}

ObjetId Etat::piocher(JoueurId joueur) {
    std::vector<ObjetId>& bibliotheque = joueurs.at(joueur).zone(Zone::Bibliotheque);
    if (bibliotheque.empty()) return kAucunObjet;

    const ObjetId id = bibliotheque.back();
    deplacer(id, Zone::Main);
    return id;
}

void Etat::engager(ObjetId id, bool engage) {
    objets.at(id).engage = engage;
}

void Etat::changerVie(JoueurId joueur, int delta) {
    joueurs.at(joueur).vie += delta;
}

void Etat::finDuTour() {
    joueurActif = (joueurActif + 1) % nbJoueurs();
    ++tour;
}

}
