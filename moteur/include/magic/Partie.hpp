#pragma once

#include <memory>
#include <optional>
#include <random>
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
    std::string refus;                
    std::vector<Evenement> evenements; 
};


class Partie {
public:
    
    Partie(const std::vector<Deck>& decks, unsigned graine,
           std::unique_ptr<Arbitre> arbitre = std::make_unique<ArbitrePermissif>(),
           int mainInitiale = 7);

    Resultat appliquer(JoueurId joueur, const Action& action);

    const Etat& etat() const { return etat_; }
    const std::vector<Evenement>& journal() const { return journal_; }

private:
    // Une surcharge par type d'action ; renvoie la raison du refus, ou nullopt si exécutée.
    std::optional<std::string> executer(JoueurId joueur, const Piocher&);
    std::optional<std::string> executer(JoueurId joueur, const Deplacer&);
    std::optional<std::string> executer(JoueurId joueur, const Engager&);
    std::optional<std::string> executer(JoueurId joueur, const ChangerVie&);
    std::optional<std::string> executer(JoueurId joueur, const FinDuTour&);

    // Ajoute une ligne au journal et renvoie son id.
    int noter(std::string regle, std::string texte, int cause = -1);

    Etat etat_;
    std::vector<Evenement> journal_;
    std::unique_ptr<Arbitre> arbitre_;
    std::mt19937 hasard_;
};

}  // namespace magic
