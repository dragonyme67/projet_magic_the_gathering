#pragma once

#include <array>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "magic/Carte.hpp"

namespace magic {

using JoueurId = int;  
using ObjetId = int;  


enum class Zone { Bibliotheque, Main, ChampDeBataille, Cimetiere, Exil, Nombre };
constexpr std::size_t kNbZones = static_cast<std::size_t>(Zone::Nombre);
const char* nomZone(Zone zone);

// Un exemplaire d'une carte en jeu.
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

    std::vector<ObjetId>& zone(Zone z) { return zones[static_cast<std::size_t>(z)]; }
    const std::vector<ObjetId>& zone(Zone z) const { return zones[static_cast<std::size_t>(z)]; }
};

// L'état complet d'une partie. C'est une valeur : copiable (retour arrière, sauvegarde)
// et sans pointeur vers l'extérieur. Les méthodes ci-dessous sont les PRIMITIVES du jeu :
// plus tard, ce sont elles que les règles appelleront. Elles ne vérifient aucune règle,
// seulement la cohérence (id valide) ; un id invalide lève std::out_of_range.
struct Etat {
    std::vector<Joueur> joueurs;
    std::vector<Objet> objets;  // indexé par ObjetId, jamais supprimé (les jetons s'ajoutent)
    JoueurId joueurActif = 0;
    int tour = 1;

    bool joueurValide(JoueurId id) const;
    bool objetValide(ObjetId id) const;

    ObjetId creerObjet(std::shared_ptr<const Carte> carte, JoueurId proprietaire, Zone zone);


    void deplacer(ObjetId id, Zone destination);
    std::optional<ObjetId> piocher(JoueurId joueur);
    void engager(ObjetId id, bool engage);
    void changerVie(JoueurId joueur, int delta);
    void finDuTour();
};

}  // namespace magic
