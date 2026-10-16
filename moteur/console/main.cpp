// Console de jeu : on joue une partie sans règles à la main, les deux joueurs sur le même
// clavier. Les commandes sont traduites en Action et envoyées à Partie::appliquer.
//
//   console_moteur [decks.json] [database.sqlite] [graine]     (à lancer depuis la racine du projet)

#include <algorithm>
#include <cctype>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Affichage.hpp"
#include "Chargeurs.hpp"
#include "magic/Catalogue.hpp"
#include "magic/Partie.hpp"

using namespace magic;

namespace {

const std::vector<std::pair<std::string, Zone>> kNomsDeZones = {
    {"bibliotheque", Zone::Bibliotheque}, {"bibliothèque", Zone::Bibliotheque}, {"biblio", Zone::Bibliotheque},
    {"main", Zone::Main},
    {"champ", Zone::ChampDeBataille}, {"champ_de_bataille", Zone::ChampDeBataille},
    {"cimetiere", Zone::Cimetiere}, {"cimetière", Zone::Cimetiere},
    {"exil", Zone::Exil},
};

std::string minuscules(std::string texte) {
    std::transform(texte.begin(), texte.end(), texte.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return texte;
}

std::optional<int> entier(const std::string& texte) {
    try {
        std::size_t lus = 0;
        const int valeur = std::stoi(texte, &lus);
        if (lus == texte.size()) return valeur;
    } catch (const std::exception&) {}
    return std::nullopt;
}

std::optional<Zone> zoneNommee(const std::string& texte) {
    const std::string cle = minuscules(texte);
    for (const auto& [nom, zone] : kNomsDeZones) {
        if (nom == cle) return zone;
    }
    return std::nullopt;
}

// Un joueur se désigne par son nom (casse ignorée) ou par son rang (1, 2…).
std::optional<JoueurId> joueurNomme(const Etat& etat, const std::string& texte) {
    if (const auto rang = entier(texte)) {
        if (*rang >= 1 && *rang <= static_cast<int>(etat.joueurs.size())) return *rang - 1;
        return std::nullopt;
    }
    for (JoueurId id = 0; id < static_cast<JoueurId>(etat.joueurs.size()); ++id) {
        if (minuscules(etat.joueurs[id].nom) == minuscules(texte)) return id;
    }
    return std::nullopt;
}

// Cartes qu'on n'a pas le droit de voir : les bibliothèques et la main de l'adversaire.
bool cachee(const Etat& etat, const Objet& objet) {
    return objet.zone == Zone::Bibliotheque ||
           (objet.zone == Zone::Main && objet.proprietaire != etat.joueurActif);
}

void appliquer(Partie& partie, const Action& action) {
    const JoueurId joueur = partie.etat().joueurActif;
    const Resultat resultat = partie.appliquer(joueur, action);
    if (!resultat.accepte) {
        std::cout << "Refusé : " << resultat.refus << '\n';
        return;
    }
    for (const Evenement& e : resultat.evenements) std::cout << "  • " << e.texte << '\n';
    afficherEtat(std::cout, partie.etat());
}

// Lit les arguments d'une commande : renvoie faux (et affiche l'usage) s'il en manque.
struct Arguments {
    std::vector<std::string> mots;
    bool exige(std::size_t minimum, const char* usage) const {
        if (mots.size() >= minimum) return true;
        std::cout << "Usage : " << usage << '\n';
        return false;
    }
};

// Objet désigné par un id tapé par le joueur, ou nullptr (message affiché).
const Objet* objetDesigne(const Etat& etat, const std::string& texte) {
    const auto id = entier(texte);
    if (!id || !etat.objetValide(*id)) {
        std::cout << "Carte inconnue : " << texte << '\n';
        return nullptr;
    }
    return &etat.objets[*id];
}

// Renvoie faux pour quitter.
bool traiter(Partie& partie, const std::string& commande, const Arguments& args) {
    const Etat& etat = partie.etat();

    if (commande == "quitter" || commande == "q") return false;

    if (commande == "aide" || commande == "?") {
        afficherAide(std::cout);
    } else if (commande == "etat") {
        afficherEtat(std::cout, etat);
    } else if (commande == "piocher" || commande == "p") {
        appliquer(partie, Piocher{});
    } else if (commande == "jouer" || commande == "j") {
        if (!args.exige(1, "jouer <id>")) return true;
        if (const Objet* objet = objetDesigne(etat, args.mots[0])) {
            appliquer(partie, Deplacer{objet->id, Zone::ChampDeBataille});
        }
    } else if (commande == "deplacer" || commande == "d") {
        if (!args.exige(2, "deplacer <id> <zone>")) return true;
        const auto zone = zoneNommee(args.mots[1]);
        if (!zone) {
            std::cout << "Zone inconnue : " << args.mots[1] << " (bibliotheque, main, champ, cimetiere, exil)\n";
        } else if (const Objet* objet = objetDesigne(etat, args.mots[0])) {
            appliquer(partie, Deplacer{objet->id, *zone});
        }
    } else if (commande == "engager" || commande == "e" || commande == "degager" || commande == "g") {
        if (!args.exige(1, "engager <id>  |  degager <id>")) return true;
        if (const Objet* objet = objetDesigne(etat, args.mots[0])) {
            appliquer(partie, Engager{objet->id, commande == "engager" || commande == "e"});
        }
    } else if (commande == "vie") {
        if (!args.exige(1, "vie <n> [joueur]")) return true;
        const auto delta = entier(args.mots[0]);
        if (!delta) {
            std::cout << "Nombre invalide : " << args.mots[0] << '\n';
            return true;
        }
        std::optional<JoueurId> cible = etat.joueurActif;
        if (args.mots.size() > 1) cible = joueurNomme(etat, args.mots[1]);
        if (!cible) {
            std::cout << "Joueur inconnu : " << args.mots[1] << '\n';
            return true;
        }
        appliquer(partie, ChangerVie{*cible, *delta});
    } else if (commande == "fin" || commande == "f") {
        appliquer(partie, FinDuTour{});
    } else if (commande == "carte" || commande == "c") {
        if (!args.exige(1, "carte <id>")) return true;
        if (const Objet* objet = objetDesigne(etat, args.mots[0])) {
            if (cachee(etat, *objet)) std::cout << "Information cachée.\n";
            else afficherCarte(std::cout, *objet);
        }
    } else if (commande == "voir" || commande == "v") {
        if (!args.exige(1, "voir <zone> [joueur]")) return true;
        const auto zone = zoneNommee(args.mots[0]);
        if (!zone) {
            std::cout << "Zone inconnue : " << args.mots[0] << '\n';
            return true;
        }
        std::optional<JoueurId> proprietaire = etat.joueurActif;
        if (args.mots.size() > 1) proprietaire = joueurNomme(etat, args.mots[1]);
        if (!proprietaire) {
            std::cout << "Joueur inconnu : " << args.mots[1] << '\n';
        } else if (*zone == Zone::Bibliotheque || (*zone == Zone::Main && *proprietaire != etat.joueurActif)) {
            std::cout << "Information cachée.\n";
        } else {
            afficherZone(std::cout, etat, *proprietaire, *zone);
        }
    } else if (commande == "journal") {
        const auto& journal = partie.journal();
        const std::size_t voulu = args.mots.empty() ? 10 : static_cast<std::size_t>(std::max(0, entier(args.mots[0]).value_or(10)));
        const std::size_t debut = journal.size() > voulu ? journal.size() - voulu : 0;
        for (std::size_t i = debut; i < journal.size(); ++i) {
            std::cout << "  [" << journal[i].id << "] " << journal[i].texte << '\n';
        }
    } else {
        std::cout << "Commande inconnue : " << commande << " (tapez « aide »)\n";
    }
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::string fichierDecks = argc > 1 ? argv[1] : "data/decks.json";
        const std::string baseCartes = argc > 2 ? argv[2] : "data/database.sqlite";
        const unsigned graine = argc > 3 ? static_cast<unsigned>(std::stoul(argv[3])) : std::random_device{}();

        const Catalogue catalogue = chargerCatalogue(baseCartes);
        const std::vector<Deck> decks = chargerDecks(fichierDecks, catalogue);

        Partie partie(decks, graine);
        std::cout << catalogue.taille() << " cartes en base · graine " << graine
                  << " (relancer avec cette graine pour rejouer le même mélange)\n";
        for (const Evenement& e : partie.journal()) std::cout << "  • " << e.texte << '\n';
        afficherEtat(std::cout, partie.etat());
        std::cout << "\nTapez « aide » pour la liste des commandes.\n";

        std::string ligne;
        while (true) {
            const Etat& etat = partie.etat();
            std::cout << '\n' << etat.joueurs[etat.joueurActif].nom << "> " << std::flush;
            if (!std::getline(std::cin, ligne)) break;  // fin de l'entrée (Ctrl+D)

            std::istringstream flux(ligne);
            std::string commande;
            if (!(flux >> commande)) continue;

            Arguments args;
            for (std::string mot; flux >> mot;) args.mots.push_back(mot);
            if (!traiter(partie, minuscules(commande), args)) break;
        }
        std::cout << "\nPartie terminée.\n";
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << '\n';
        return 1;
    }
    return 0;
}
