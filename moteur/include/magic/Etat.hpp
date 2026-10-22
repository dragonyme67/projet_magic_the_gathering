#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "magic/Carte.hpp"

namespace magic {

using JoueurId = int;  // indice dans Etat::joueurs
using ObjetId = int;   // indice dans Etat::objets

const ObjetId kAucunObjet = -1;

enum class Zone { Bibliotheque, Main, ChampDeBataille, Cimetiere, Exil, Nombre };
constexpr int kNbZones = static_cast<int>(Zone::Nombre);
const char* nomZone(Zone zone);

// Exemplaire d'une carte en jeu
struct Objet {
    ObjetId id = 0;
    std::shared_ptr<const Carte> carte;
    JoueurId proprietaire = 0;
    Zone zone = Zone::Bibliotheque;
    bool malInvocation = false;
    bool engage = false;
};

struct Joueur {
    std::string nom;
    int vie = 20;
    // bibliothèque : le dessus est la fin du vecteur
    std::array<std::vector<ObjetId>, kNbZones> zones;

    std::vector<ObjetId>& zone(Zone z) { return zones[static_cast<int>(z)]; }
    const std::vector<ObjetId>& zone(Zone z) const { return zones[static_cast<int>(z)]; }
};

struct Etat {
    std::vector<Joueur> joueurs;
    std::vector<Objet> objets;  
    JoueurId joueurActif = 0;
    int tour = 1;

    int nbJoueurs() const { return static_cast<int>(joueurs.size()); }
    int nbObjets() const { return static_cast<int>(objets.size()); }
    bool joueurValide(JoueurId id) const { return id >= 0 && id < nbJoueurs(); }
    bool objetValide(ObjetId id) const { return id >= 0 && id < nbObjets(); }

    ObjetId creerObjet(std::shared_ptr<const Carte> carte, JoueurId proprietaire, Zone zone);

    void deplacer(ObjetId id, Zone destination);
    ObjetId piocher(JoueurId joueur);
    void engager(ObjetId id, bool engage);
    bool estEngage(ObjetId id) const;
    bool aMalInvocation(ObjetId id) const;
    void changerVie(JoueurId joueur, int delta);
    void finDuTour();
};

}
