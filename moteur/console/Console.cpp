#include "Console.hpp"

#include <cctype>
#include <iostream>
#include <sstream>

#include "Affichage.hpp"

using namespace magic;

namespace {

struct NomDeZone {
    const char* nom;
    Zone zone;
};

// Façons d'écrire une zone dans une commande.
const NomDeZone kNomsDeZones[] = {
    {"bibliotheque", Zone::Bibliotheque}, {"bibliothèque", Zone::Bibliotheque}, {"biblio", Zone::Bibliotheque},
    {"main", Zone::Main},
    {"champ", Zone::ChampDeBataille}, {"champ_de_bataille", Zone::ChampDeBataille},
    {"cimetiere", Zone::Cimetiere}, {"cimetière", Zone::Cimetiere},
    {"exil", Zone::Exil},
};

std::string minuscules(std::string texte) {
    const int taille = texte.size();
    for (int i = 0; i < taille; ++i) {
        texte[i] = std::tolower(static_cast<unsigned char>(texte[i]));
    }
    return texte;
}

// Convertit un texte en entier. Renvoie faux si ce n'est pas un nombre (« 12x » est refusé).
bool lireEntier(const std::string& texte, int& valeur) {
    std::istringstream flux(texte);
    char reste;
    return (flux >> valeur) && !(flux >> reste);
}

// Renvoie Zone::Nombre si le nom est inconnu.
Zone zoneNommee(const std::string& texte) {
    const std::string cle = minuscules(texte);
    for (const NomDeZone& n : kNomsDeZones) {
        if (cle == n.nom) return n.zone;
    }
    return Zone::Nombre;
}

// Un joueur se désigne par son nom (majuscules ignorées) ou par son rang : 1, 2.
// Renvoie -1 si on ne le trouve pas.
JoueurId joueurNomme(const Etat& etat, const std::string& texte) {
    int rang = 0;
    if (lireEntier(texte, rang)) {
        if (rang >= 1 && rang <= etat.nbJoueurs()) return rang - 1;
        return -1;
    }
    for (JoueurId id = 0; id < etat.nbJoueurs(); ++id) {
        if (minuscules(etat.joueurs[id].nom) == minuscules(texte)) return id;
    }
    return -1;
}

// Informations cachées : les bibliothèques et la main de l'adversaire.
bool zoneCachee(const Etat& etat, JoueurId proprietaire, Zone zone) {
    if (zone == Zone::Bibliotheque) return true;
    return zone == Zone::Main && proprietaire != etat.joueurActif;
}

}  // namespace

Console::Console(Game& jeu) : jeu_(jeu) {}

void Console::run() {
    if (!jeu_.enCours()) jeu_.lancer();

    std::cout << jeu_.catalogue().taille() << " cartes en base · graine " << jeu_.graine()
              << " (relancer avec cette graine pour rejouer le même mélange)\n";
    for (const Evenement& e : jeu_.journal()) std::cout << "  • " << e.texte << '\n';
    afficherEtat(std::cout, jeu_.etat());
    std::cout << "\nTapez « aide » pour la liste des commandes.\n";

    std::string ligne;
    while (true) {
        const Etat& etat = jeu_.etat();
        std::cout << '\n' << etat.joueurs[etat.joueurActif].nom << "> " << std::flush;
        if (!std::getline(std::cin, ligne)) break;  // fin de l'entrée (Ctrl+D)

        // On découpe la ligne en mots : le premier est la commande, les autres les arguments.
        std::istringstream flux(ligne);
        std::string commande;
        flux >> commande;
        if (commande.empty()) continue;

        std::vector<std::string> args;
        std::string mot;
        while (flux >> mot) args.push_back(mot);

        if (!traiter(minuscules(commande), args)) break;
    }
    std::cout << "\nPartie terminée.\n";
}

bool Console::traiter(const std::string& commande, const std::vector<std::string>& args) {
    if (commande == "quitter" || commande == "q") return false;

    if (commande == "aide" || commande == "?") afficherAide(std::cout);
    else if (commande == "etat") afficherEtat(std::cout, jeu_.etat());
    else if (commande == "piocher" || commande == "p") appliquer(Action::piocher());
    else if (commande == "jouer" || commande == "j") jouer(args);
    else if (commande == "deplacer" || commande == "d") deplacer(args);
    else if (commande == "engager" || commande == "e") engager(args, true);
    else if (commande == "degager" || commande == "g") engager(args, false);
    else if (commande == "vie") vie(args);
    else if (commande == "fin" || commande == "f") appliquer(Action::finDuTour());
    else if (commande == "carte" || commande == "c") carte(args);
    else if (commande == "voir" || commande == "v") voir(args);
    else if (commande == "journal") journal(args);
    else std::cout << "Commande inconnue : " << commande << " (tapez « aide »)\n";

    return true;
}

// --- Une fonction par commande ---------------------------------------------------

void Console::jouer(const std::vector<std::string>& args) {
    if (!argumentsSuffisants(args, 1, "jouer <id>")) return;

    ObjetId id = kAucunObjet;
    if (lireObjet(args[0], id)) appliquer(Action::deplacer(id, Zone::ChampDeBataille));
}

void Console::deplacer(const std::vector<std::string>& args) {
    if (!argumentsSuffisants(args, 2, "deplacer <id> <zone>")) return;

    const Zone zone = zoneNommee(args[1]);
    if (zone == Zone::Nombre) {
        std::cout << "Zone inconnue : " << args[1] << " (bibliotheque, main, champ, cimetiere, exil)\n";
        return;
    }
    ObjetId id = kAucunObjet;
    if (lireObjet(args[0], id)) appliquer(Action::deplacer(id, zone));
}

void Console::engager(const std::vector<std::string>& args, bool engage) {
    if (!argumentsSuffisants(args, 1, "engager <id>  |  degager <id>")) return;

    ObjetId id = kAucunObjet;
    if (!lireObjet(args[0], id)) return;
    if (engage) appliquer(Action::engager(id));
    else appliquer(Action::degager(id));
}

void Console::vie(const std::vector<std::string>& args) {
    if (!argumentsSuffisants(args, 1, "vie <n> [joueur]")) return;

    int delta = 0;
    if (!lireEntier(args[0], delta)) {
        std::cout << "Nombre invalide : " << args[0] << '\n';
        return;
    }

    const Etat& etat = jeu_.etat();
    JoueurId cible = etat.joueurActif;  // par défaut : le joueur dont c'est le tour
    if (args.size() > 1) {
        cible = joueurNomme(etat, args[1]);
        if (cible < 0) {
            std::cout << "Joueur inconnu : " << args[1] << '\n';
            return;
        }
    }
    appliquer(Action::changerVie(cible, delta));
}

void Console::carte(const std::vector<std::string>& args) {
    if (!argumentsSuffisants(args, 1, "carte <id>")) return;

    ObjetId id = kAucunObjet;
    if (!lireObjet(args[0], id)) return;

    const Etat& etat = jeu_.etat();
    const Objet& objet = etat.objets[id];
    if (zoneCachee(etat, objet.proprietaire, objet.zone)) std::cout << "Information cachée.\n";
    else afficherCarte(std::cout, objet);
}

void Console::voir(const std::vector<std::string>& args) {
    if (!argumentsSuffisants(args, 1, "voir <zone> [joueur]")) return;

    const Zone zone = zoneNommee(args[0]);
    if (zone == Zone::Nombre) {
        std::cout << "Zone inconnue : " << args[0] << '\n';
        return;
    }

    const Etat& etat = jeu_.etat();
    JoueurId proprietaire = etat.joueurActif;
    if (args.size() > 1) {
        proprietaire = joueurNomme(etat, args[1]);
        if (proprietaire < 0) {
            std::cout << "Joueur inconnu : " << args[1] << '\n';
            return;
        }
    }

    if (zoneCachee(etat, proprietaire, zone)) std::cout << "Information cachée.\n";
    else afficherZone(std::cout, etat, proprietaire, zone);
}

void Console::journal(const std::vector<std::string>& args) {
    int voulu = 10;  // nombre d'événements à montrer
    if (!args.empty() && !lireEntier(args[0], voulu)) {
        std::cout << "Nombre invalide : " << args[0] << '\n';
        return;
    }

    const std::vector<Evenement>& evenements = jeu_.journal();
    const int total = evenements.size();
    int debut = total - voulu;
    if (debut < 0) debut = 0;
    for (int i = debut; i < total; ++i) {
        std::cout << "  [" << evenements[i].id << "] " << evenements[i].texte << '\n';
    }
}

// --- Outils ----------------------------------------------------------------------

void Console::appliquer(const Action& action) {
    const JoueurId joueur = jeu_.etat().joueurActif;
    const Resultat resultat = jeu_.appliquer(joueur, action);

    if (!resultat.accepte) {
        std::cout << "Refusé : " << resultat.refus << '\n';
        return;
    }
    for (const Evenement& e : resultat.evenements) std::cout << "  • " << e.texte << '\n';
    afficherEtat(std::cout, jeu_.etat());
}

bool Console::argumentsSuffisants(const std::vector<std::string>& args, int minimum,
                                  const char* usage) const {
    const int nombre = args.size();
    if (nombre >= minimum) return true;
    std::cout << "Usage : " << usage << '\n';
    return false;
}

bool Console::lireObjet(const std::string& texte, ObjetId& id) const {
    int valeur = 0;
    if (!lireEntier(texte, valeur) || !jeu_.etat().objetValide(valeur)) {
        std::cout << "Carte inconnue : " << texte << '\n';
        return false;
    }
    id = valeur;
    return true;
}
