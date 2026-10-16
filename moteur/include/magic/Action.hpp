#pragma once

#include "magic/Etat.hpp"

namespace magic {

// Pour ajouter une action : une valeur ici, un champ dans Action si besoin, une fonction
// statique pour la créer, puis un cas dans Partie::executer (le compilateur prévient si on
// l'oublie).
enum class TypeAction { Piocher, Deplacer, Engager, Degager, ChangerVie, FinDuTour };

// Ce qu'un joueur demande. Chaque type n'utilise que certains champs :
//   Deplacer          : objet, zone (destination)
//   Engager / Degager : objet
//   ChangerVie        : cible (le joueur qui gagne ou perd de la vie), delta
// On crée une action avec les fonctions ci-dessous, par exemple Action::deplacer(12, Zone::Main).
struct Action {
    TypeAction type = TypeAction::Piocher;
    ObjetId objet = kAucunObjet;
    Zone zone = Zone::Main;
    JoueurId cible = 0;
    int delta = 0;

    static Action piocher() {
        Action a;
        a.type = TypeAction::Piocher;
        return a;
    }
    static Action deplacer(ObjetId objet, Zone zone) {
        Action a;
        a.type = TypeAction::Deplacer;
        a.objet = objet;
        a.zone = zone;
        return a;
    }
    static Action engager(ObjetId objet) {
        Action a;
        a.type = TypeAction::Engager;
        a.objet = objet;
        return a;
    }
    static Action degager(ObjetId objet) {
        Action a;
        a.type = TypeAction::Degager;
        a.objet = objet;
        return a;
    }
    static Action changerVie(JoueurId cible, int delta) {
        Action a;
        a.type = TypeAction::ChangerVie;
        a.cible = cible;
        a.delta = delta;
        return a;
    }
    static Action finDuTour() {
        Action a;
        a.type = TypeAction::FinDuTour;
        return a;
    }
};

}  // namespace magic
