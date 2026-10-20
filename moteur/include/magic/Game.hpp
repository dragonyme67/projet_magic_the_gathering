#pragma once

#include <memory>
#include <vector>

#include "magic/Action.hpp"
#include "magic/Carte.hpp"
#include "magic/Etat.hpp"
#include "magic/Evenement.hpp"
#include "magic/Partie.hpp"

namespace magic {

// Point d'entrée du moteur (console, Drogon). Une partie à la fois, pas thread-safe.
class Game {
public:
    explicit Game(const std::vector<Deck>& decks);

    // sans graine : l'heure
    void lancer();
    void lancer(unsigned graine);
    void terminer();

    bool enCours() const { return partie_ != nullptr; }
    unsigned graine() const { return graine_; }

    // sans partie : refus « Aucune partie en cours »
    Resultat appliquer(JoueurId joueur, const Action& action);

    // sans partie : std::runtime_error
    const Etat& etat() const;
    const std::vector<Evenement>& journal() const;

private:
    void exigerPartie() const;

    std::vector<Deck> decks_;
    std::unique_ptr<Partie> partie_;
    unsigned graine_ = 0;
};

}
