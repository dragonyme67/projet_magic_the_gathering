// Console de jeu : on joue une partie sans règles à la main, les deux joueurs sur le même clavier.
//
//   console_moteur [decks.json] [database.sqlite] [graine]     (à lancer depuis la racine du projet)

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Chargeurs.hpp"
#include "Console.hpp"
#include "magic/Game.hpp"

int main(int argc, char** argv) {
    try {
        std::string fichierDecks = "data/decks.json";
        std::string baseCartes = "data/database.sqlite";
        if (argc > 1) fichierDecks = argv[1];
        if (argc > 2) baseCartes = argv[2];

        // La console charge les cartes et les decks, puis donne le tout à Game.
        magic::Catalogue catalogue = chargerCatalogue(baseCartes);
        std::vector<magic::Deck> decks = chargerDecks(fichierDecks, catalogue);
        magic::Game jeu(catalogue, decks);

        if (argc > 3) {
            unsigned graine = 0;
            char reste;
            std::istringstream flux(argv[3]);
            if (!(flux >> graine) || (flux >> reste)) {
                std::cerr << "Graine invalide : " << argv[3] << " (un nombre entier est attendu)\n";
                return 1;
            }
            jeu.lancer(graine);
        } else {
            jeu.lancer();
        }

        Console console(jeu);
        console.run();
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << '\n';
        return 1;
    }
    return 0;
}
