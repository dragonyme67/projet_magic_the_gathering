#include "magic/Partie.hpp"

#include <algorithm>
#include <utility>

namespace magic {

Partie::Partie(const std::vector<Deck>& decks, unsigned graine,
               std::unique_ptr<Arbitre> arbitre, int mainInitiale)
    : arbitre_(std::move(arbitre)), hasard_(graine) {
    for (const Deck& deck : decks) {
        Joueur joueur;
        joueur.nom = deck.joueur;
        etat_.joueurs.push_back(std::move(joueur));
    }

    for (std::size_t i = 0; i < decks.size(); ++i) {
        const JoueurId id = static_cast<JoueurId>(i);
        for (const Carte& carte : decks[i].cartes) {
            etat_.creerObjet(std::make_shared<const Carte>(carte), id, Zone::Bibliotheque);
        }

        auto& bibliotheque = etat_.joueurs[i].zone(Zone::Bibliotheque);
        std::shuffle(bibliotheque.begin(), bibliotheque.end(), hasard_);

        int piochees = 0;
        for (; piochees < mainInitiale && etat_.piocher(id); ++piochees) {}
        noter("mise_en_place", decks[i].joueur + " mélange son deck et pioche " +
                                   std::to_string(piochees) + " cartes.");
    }
}

Resultat Partie::appliquer(JoueurId joueur, const Action& action) {
    Resultat resultat;
    const std::size_t debut = journal_.size();

    auto refuser = [&](std::string raison) {
        resultat.refus = std::move(raison);
        return resultat;
    };

    if (!etat_.joueurValide(joueur)) return refuser("Joueur inconnu.");
    if (auto raison = arbitre_->valider(etat_, joueur, action)) return refuser(*raison);

    const std::optional<std::string> echec =
        std::visit([&](const auto& a) { return executer(joueur, a); }, action);
    if (echec) return refuser(*echec);

    resultat.accepte = true;
    resultat.evenements.assign(journal_.begin() + static_cast<std::ptrdiff_t>(debut), journal_.end());
    return resultat;
}

// --- Actions -----------------------------------------------------------------
// Pour l'instant chaque action appelle directement une primitive de Etat, sans règle.
// Les règles viendront s'intercaler ici (ou dans l'Arbitre) sans changer l'API de Partie.

std::optional<std::string> Partie::executer(JoueurId joueur, const Piocher&) {
    const auto objet = etat_.piocher(joueur);
    if (!objet) return "La bibliothèque est vide.";
    noter("piocher", etat_.joueurs[joueur].nom + " pioche " + etat_.objets[*objet].carte->nomAffiche() + ".");
    return std::nullopt;
}

std::optional<std::string> Partie::executer(JoueurId joueur, const Deplacer& a) {
    if (!etat_.objetValide(a.objet)) return "Objet inconnu.";
    if (a.destination == Zone::Nombre) return "Zone inconnue.";

    const Objet& objet = etat_.objets[a.objet];
    const Zone origine = objet.zone;
    etat_.deplacer(a.objet, a.destination);
    noter("deplacer", etat_.joueurs[joueur].nom + " déplace " + objet.carte->nomAffiche() + " : " +
                          nomZone(origine) + " → " + nomZone(a.destination) + ".");
    return std::nullopt;
}

std::optional<std::string> Partie::executer(JoueurId joueur, const Engager& a) {
    if (!etat_.objetValide(a.objet)) return "Objet inconnu.";

    etat_.engager(a.objet, a.engage);
    noter("engager", etat_.joueurs[joueur].nom + (a.engage ? " engage " : " dégage ") +
                         etat_.objets[a.objet].carte->nomAffiche() + ".");
    return std::nullopt;
}

std::optional<std::string> Partie::executer(JoueurId, const ChangerVie& a) {
    if (!etat_.joueurValide(a.cible)) return "Joueur inconnu.";

    etat_.changerVie(a.cible, a.delta);
    const Joueur& cible = etat_.joueurs[a.cible];
    noter("changer_vie", cible.nom + (a.delta >= 0 ? " gagne " : " perd ") +
                             std::to_string(a.delta >= 0 ? a.delta : -a.delta) +
                             " points de vie (" + std::to_string(cible.vie) + ").");
    return std::nullopt;
}

std::optional<std::string> Partie::executer(JoueurId joueur, const FinDuTour&) {
    const std::string sortant = etat_.joueurs[joueur].nom;
    etat_.finDuTour();
    noter("fin_du_tour", "Fin du tour de " + sortant + ". Tour " + std::to_string(etat_.tour) +
                             " : " + etat_.joueurs[etat_.joueurActif].nom + " joue.");
    return std::nullopt;
}

int Partie::noter(std::string regle, std::string texte, int cause) {
    const int id = static_cast<int>(journal_.size());
    journal_.push_back(Evenement{id, cause, std::move(regle), std::move(texte)});
    return id;
}

}  // namespace magic
