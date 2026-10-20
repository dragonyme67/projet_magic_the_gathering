#pragma once

#include <memory>
#include <string>
#include <vector>

#include "magic/Action.hpp"
#include "magic/Arbitre.hpp"
#include "magic/Carte.hpp"
#include "magic/Etat.hpp"
#include "magic/Evenement.hpp"

namespace magic {

struct Resultat {
    bool accepte = false;
    std::string refus;  // vide si accepté
    std::vector<Evenement> evenements;
};

class Partie {
public:
    // un joueur par deck ; même graine = même mélange
    Partie(const std::vector<Deck>& decks, unsigned graine,
           std::unique_ptr<Arbitre> arbitre = std::make_unique<ArbitrePermissif>(),
           int mainInitiale = 7);

    Resultat appliquer(JoueurId joueur, const Action& action);

    const Etat& etat() const { return etat_; }
    const std::vector<Evenement>& journal() const { return journal_; }

private:
    // "" = fait, sinon la raison du refus
    std::string executer(JoueurId joueur, const Action& action);
    std::string executerPiocher(JoueurId joueur);
    std::string executerDeplacer(JoueurId joueur, const Action& action);
    std::string executerEngager(JoueurId joueur, const Action& action, bool engage);
    std::string executerChangerVie(const Action& action);
    std::string executerFinDuTour(JoueurId joueur);

    int noter(const std::string& regle, const std::string& texte, int cause = -1);

    int tirer(int max);  // 0 .. max-1
    void melanger(std::vector<ObjetId>& cartes);

    Etat etat_;
    std::vector<Evenement> journal_;
    std::unique_ptr<Arbitre> arbitre_;
};

}
