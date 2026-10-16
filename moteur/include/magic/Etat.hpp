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

// Pour ajouter une zone : une valeur ici (avant Nombre) et son nom dans Etat.cpp.
enum class Zone { Bibliotheque, Main, ChampDeBataille, Cimetiere, Exil, Nombre };
constexpr int kNbZones = static_cast<int>(Zone::Nombre);
const char* nomZone(Zone zone);

// Un exemplaire d'une carte en jeu. Plusieurs objets peuvent partager la même Carte.
struct Objet {
    ObjetId id = 0;
    std::shared_ptr<const Carte> carte;
    JoueurId proprietaire = 0;
    Zone zone = Zone::Bibliotheque;
    bool engage = false;
};

struct Joueur {
    std::string nom;
    int vie = 20;
    // Contenu de chaque zone, dans l'ordre. Bibliothèque : le DESSUS est la fin du vecteur.
    std::array<std::vector<ObjetId>, kNbZones> zones;

    std::vector<ObjetId>& zone(Zone z) { return zones[static_cast<int>(z)]; }
    const std::vector<ObjetId>& zone(Zone z) const { return zones[static_cast<int>(z)]; }
};

// L'état complet d'une partie. Les méthodes sont les opérations de base du jeu : plus tard,
// les règles les appelleront. Elles ne vérifient aucune règle, seulement que les ids existent
// (sinon std::out_of_range).
struct Etat {
    std::vector<Joueur> joueurs;
    std::vector<Objet> objets;  // indexé par ObjetId, on n'en supprime jamais
    JoueurId joueurActif = 0;
    int tour = 1;

    int nbJoueurs() const { return static_cast<int>(joueurs.size()); }
    int nbObjets() const { return static_cast<int>(objets.size()); }
    bool joueurValide(JoueurId id) const { return id >= 0 && id < nbJoueurs(); }
    bool objetValide(ObjetId id) const { return id >= 0 && id < nbObjets(); }

    ObjetId creerObjet(std::shared_ptr<const Carte> carte, JoueurId proprietaire, Zone zone);

    // La carte va dans la zone de son propriétaire.
    void deplacer(ObjetId id, Zone destination);
    // Prend la carte du dessus de la bibliothèque et la met en main.
    // Renvoie son id, ou kAucunObjet si la bibliothèque est vide.
    ObjetId piocher(JoueurId joueur);
    void engager(ObjetId id, bool engage);
    void changerVie(JoueurId joueur, int delta);
    // Passe la main au joueur suivant et augmente le numéro de tour.
    void finDuTour();
};

}  // namespace magic
