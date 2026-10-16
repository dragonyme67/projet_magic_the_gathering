#include "magic/Partie.hpp"

#include <utility>

namespace magic {

Partie::Partie(const std::vector<Deck>& decks, unsigned graine,
               std::unique_ptr<Arbitre> arbitre, int mainInitiale)
    : arbitre_(std::move(arbitre)), hasard_(graine) {
    // Un joueur par deck.
    for (const Deck& deck : decks) {
        Joueur joueur;
        joueur.nom = deck.joueur;
        etat_.joueurs.push_back(joueur);
    }

    // Pour chaque joueur : ses cartes en bibliothèque, mélange, main de départ.
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

    // L'arbitre d'abord, puis l'exécution. Une chaîne vide veut dire « pas de refus ».
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

// --- Actions -----------------------------------------------------------------
// Pour l'instant chaque action appelle directement une opération de Etat, sans règle.
// Les règles viendront s'insérer ici (ou dans l'Arbitre) sans changer le reste.

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

// --- Hasard ------------------------------------------------------------------
// Petit générateur (congruence linéaire) : à chaque appel, hasard_ est remplacé par un nouveau
// grand nombre qui paraît aléatoire mais qui ne dépend que de la graine. On garde les 16 bits
// du haut, car ceux du bas sont peu aléatoires. On ne se sert pas de rand() : il est partagé
// par tout le programme, donc deux parties simultanées (Drogon) se perturberaient.

int Partie::tirer(int max) {
    hasard_ = hasard_ * 1664525u + 1013904223u;  // le dépassement d'un unsigned est permis : le nombre « reboucle »
    return (hasard_ >> 16) % max;
}

// Mélange de Fisher-Yates : on parcourt de la fin au début et on échange chaque carte avec
// une carte tirée au hasard parmi celles qui précèdent (elle-même incluse).
void Partie::melanger(std::vector<ObjetId>& cartes) {
    const int taille = cartes.size();
    for (int i = taille - 1; i > 0; --i) {
        const int j = tirer(i + 1);
        const ObjetId temporaire = cartes[i];
        cartes[i] = cartes[j];
        cartes[j] = temporaire;
    }
}

}  // namespace magic
