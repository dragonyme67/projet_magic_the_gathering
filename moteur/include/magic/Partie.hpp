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
    std::string refus;                  // raison du refus, vide si accepté
    std::vector<Evenement> evenements;  // ce que cette action a produit
};

// Une partie en cours : l'état, le journal, l'arbitre. Game en crée une à chaque lancement.
class Partie {
public:
    // Un joueur par deck. Même graine = même mélange.
    Partie(const std::vector<Deck>& decks, unsigned graine,
           std::unique_ptr<Arbitre> arbitre = std::make_unique<ArbitrePermissif>(),
           int mainInitiale = 7);

    Resultat appliquer(JoueurId joueur, const Action& action);

    const Etat& etat() const { return etat_; }
    const std::vector<Evenement>& journal() const { return journal_; }

private:
    // Fait l'action. Renvoie la raison du refus, ou une chaîne vide si elle a été faite.
    std::string executer(JoueurId joueur, const Action& action);
    std::string executerPiocher(JoueurId joueur);
    std::string executerDeplacer(JoueurId joueur, const Action& action);
    std::string executerEngager(JoueurId joueur, const Action& action, bool engage);
    std::string executerChangerVie(const Action& action);
    std::string executerFinDuTour(JoueurId joueur);

    // Ajoute une ligne au journal et renvoie son id.
    int noter(const std::string& regle, const std::string& texte, int cause = -1);

    // Générateur de nombres pseudo-aléatoires propre à la partie (même graine, mêmes tirages).
    int tirer(int max);  // entier entre 0 et max - 1 (max doit rester sous 65536)
    void melanger(std::vector<ObjetId>& cartes);

    Etat etat_;
    std::vector<Evenement> journal_;
    std::unique_ptr<Arbitre> arbitre_;
    unsigned int hasard_;
};

}  // namespace magic
