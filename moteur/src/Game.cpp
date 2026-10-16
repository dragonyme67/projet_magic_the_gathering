#include "magic/Game.hpp"

#include <ctime>
#include <stdexcept>

namespace magic {

Game::Game(const Catalogue& catalogue, const std::vector<Deck>& decks)
    : catalogue_(catalogue), decks_(decks) {}

void Game::lancer() {
    lancer(static_cast<unsigned>(std::time(nullptr)));
}

void Game::lancer(unsigned graine) {
    graine_ = graine;
    partie_ = std::make_unique<Partie>(decks_, graine_);  // remplace l'éventuelle partie en cours
}

void Game::terminer() {
    partie_.reset();
}

Resultat Game::appliquer(JoueurId joueur, const Action& action) {
    if (!enCours()) {
        Resultat resultat;
        resultat.refus = "Aucune partie en cours.";
        return resultat;
    }
    return partie_->appliquer(joueur, action);
}

const Etat& Game::etat() const {
    exigerPartie();
    return partie_->etat();
}

const std::vector<Evenement>& Game::journal() const {
    exigerPartie();
    return partie_->journal();
}

void Game::exigerPartie() const {
    if (!enCours()) throw std::runtime_error("Aucune partie en cours.");
}

}  // namespace magic
