#include "magic/Partie.hpp"

#include <cstdlib>
#include <utility>

namespace magic {

Partie::Partie(const std::vector<Deck>& decks, unsigned graine,
               std::unique_ptr<Arbitre> arbitre, int mainInitiale)
    : arbitre_(std::move(arbitre)) {
    std::srand(graine);  // même graine = même mélange
    for (const Deck& deck : decks) {
        Joueur joueur;
        joueur.nom = deck.joueur;
        etat_.joueurs.push_back(joueur);
    }

    for (JoueurId id = 0; id < etat_.nbJoueurs(); ++id) {
        for (const Carte& carte : decks[id].cartes) {
            etat_.creerObjet(std::make_shared<const Carte>(carte), id, Zone::Bibliotheque);
        }
        melanger(etat_.joueurs[id].zone(Zone::Bibliotheque));

        int piochees = 0;
        while (piochees < mainInitiale && etat_.piocher(id) != kAucunObjet) {
            ++piochees;
        }
        noter("mise_en_place", decks[id].joueur + " mélange son deck et pioche " +
                                   std::to_string(piochees) + " cartes.");
    }
}

Resultat Partie::appliquer(JoueurId joueur, const Action& action) {
    Resultat resultat;
    const int debut = journal_.size();

    if (!etat_.joueurValide(joueur)) {
        resultat.refus = "Joueur inconnu.";
        return resultat;
    }

    std::string refus = arbitre_->valider(etat_, joueur, action);
    if (refus.empty()) refus = executer(joueur, action);
    if (!refus.empty()) {
        resultat.refus = refus;
        return resultat;
    }

    resultat.accepte = true;
    const int fin = journal_.size();
    for (int i = debut; i < fin; ++i) {
        resultat.evenements.push_back(journal_[i]);
    }
    return resultat;
}

// --- Actions (sans règle : appel direct des opérations de Etat) ---

std::string Partie::executer(JoueurId joueur, const Action& action) {
    switch (action.type) {
        case TypeAction::Piocher:    return executerPiocher(joueur);
        case TypeAction::Deplacer:   return executerDeplacer(joueur, action);
        case TypeAction::Engager:    return executerEngager(joueur, action, true);
        case TypeAction::Degager:    return executerEngager(joueur, action, false);
        case TypeAction::ChangerVie: return executerChangerVie(action);
        case TypeAction::FinDuTour:  return executerFinDuTour(joueur);
    }
    return "Action inconnue.";
}

std::string Partie::executerPiocher(JoueurId joueur) {
    const ObjetId objet = etat_.piocher(joueur);
    if (objet == kAucunObjet) return "La bibliothèque est vide.";

    noter("piocher", etat_.joueurs[joueur].nom + " pioche " + etat_.objets[objet].carte->nomAffiche() + ".");
    return "";
}

std::string Partie::executerDeplacer(JoueurId joueur, const Action& action) {
    if (!etat_.objetValide(action.objet)) return "Carte inconnue.";
    if (action.zone == Zone::Nombre) return "Zone inconnue.";

    const Objet& objet = etat_.objets[action.objet];
    const Zone origine = objet.zone;
    etat_.deplacer(action.objet, action.zone);
    noter("deplacer", etat_.joueurs[joueur].nom + " déplace " + objet.carte->nomAffiche() + " : " +
                          nomZone(origine) + " → " + nomZone(action.zone) + ".");
    return "";
}

std::string Partie::executerEngager(JoueurId joueur, const Action& action, bool engage) {
    if (!etat_.objetValide(action.objet)) return "Carte inconnue.";

    etat_.engager(action.objet, engage);
    const std::string verbe = engage ? " engage " : " dégage ";
    noter("engager", etat_.joueurs[joueur].nom + verbe + etat_.objets[action.objet].carte->nomAffiche() + ".");
    return "";
}

std::string Partie::executerChangerVie(const Action& action) {
    if (!etat_.joueurValide(action.cible)) return "Joueur inconnu.";

    etat_.changerVie(action.cible, action.delta);
    const Joueur& cible = etat_.joueurs[action.cible];
    std::string texte = cible.nom;
    if (action.delta >= 0) texte += " gagne " + std::to_string(action.delta);
    else texte += " perd " + std::to_string(-action.delta);
    texte += " points de vie (" + std::to_string(cible.vie) + ").";
    noter("changer_vie", texte);
    return "";
}

std::string Partie::executerFinDuTour(JoueurId joueur) {
    const std::string sortant = etat_.joueurs[joueur].nom;
    etat_.finDuTour();
    noter("fin_du_tour", "Fin du tour de " + sortant + ". Tour " + std::to_string(etat_.tour) + " : " +
                             etat_.joueurs[etat_.joueurActif].nom + " joue.");
    return "";
}

int Partie::noter(const std::string& regle, const std::string& texte, int cause) {
    Evenement evenement;
    evenement.id = journal_.size();
    evenement.cause = cause;
    evenement.regle = regle;
    evenement.texte = texte;
    journal_.push_back(evenement);
    return evenement.id;
}

// --- Hasard ---

int Partie::tirer(int max) {
    return std::rand() % max;
}

// Fisher-Yates
void Partie::melanger(std::vector<ObjetId>& cartes) {
    const int taille = cartes.size();
    for (int i = taille - 1; i > 0; --i) {
        const int j = tirer(i + 1);
        const ObjetId temporaire = cartes[i];
        cartes[i] = cartes[j];
        cartes[j] = temporaire;
    }
}

}
