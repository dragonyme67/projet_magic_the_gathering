#pragma once

#include "magic/Etat.hpp"

namespace magic {

enum class TypeAction { Piocher, Deplacer, Engager, Degager, ChangerVie, FinDuTour };

// Champs utilisés : Deplacer (objet, zone) · Engager/Degager (objet) · ChangerVie (cible, delta)
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

}
