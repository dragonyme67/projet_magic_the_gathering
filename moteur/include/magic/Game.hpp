#pragma once

#include <memory>
#include <vector>

#include "magic/Action.hpp"
#include "magic/Carte.hpp"
#include "magic/Catalogue.hpp"
#include "magic/Etat.hpp"
#include "magic/Evenement.hpp"
#include "magic/Partie.hpp"

namespace magic {

// Point d'entrée pour utiliser le moteur : la console s'en sert, et Drogon s'en servira.
// Game connaît le catalogue et les decks, lance une partie, et lui transmet les actions.
//
//   Game jeu(catalogue, decks);
//   jeu.lancer();                                          // nouvelle partie
//   Resultat r = jeu.appliquer(0, Action::piocher());      // le joueur 0 pioche
//   const Etat& etat = jeu.etat();
//
// Une seule partie à la fois : lancer() remplace la partie en cours.
// Game n'est pas protégé contre les appels simultanés de plusieurs threads.
class Game {
public:
    Game(const Catalogue& catalogue, const std::vector<Deck>& decks);

    // Lance une partie avec les decks. Sans graine, on utilise l'heure.
    void lancer();
    void lancer(unsigned graine);
    // Arrête la partie en cours.
    void terminer();

    bool enCours() const { return partie_ != nullptr; }
    unsigned graine() const { return graine_; }  // graine de la dernière partie lancée
    const Catalogue& catalogue() const { return catalogue_; }

    // Sans partie en cours : Resultat refusé avec le message « Aucune partie en cours ».
    Resultat appliquer(JoueurId joueur, const Action& action);

    // Sans partie en cours : lèvent std::runtime_error.
    const Etat& etat() const;
    const std::vector<Evenement>& journal() const;

private:
    void exigerPartie() const;

    Catalogue catalogue_;
    std::vector<Deck> decks_;
    std::unique_ptr<Partie> partie_;  // nullptr tant qu'aucune partie n'est lancée
    unsigned graine_ = 0;
};

}  // namespace magic
