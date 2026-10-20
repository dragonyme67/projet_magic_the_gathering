// Usage : console_moteur [graine]  (depuis la racine du projet)

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Console.hpp"
#include "magic/Game.hpp"
#include "magic/io/DeckRepository.hpp"

int main(int argc, char** argv) {
    try {
        magic::DeckRepository decks("resources", "data/database.sqlite");
        magic::Game jeu(std::vector<magic::Deck>{
            decks.charger("deck_1", "Alice"),
            decks.charger("deck_2", "Bob"),
        });

        if (argc > 1) {
            unsigned graine = 0;
            char reste;
            std::istringstream flux(argv[1]);
            if (!(flux >> graine) || (flux >> reste)) {
                std::cerr << "Graine invalide : " << argv[1] << " (un nombre entier est attendu)\n";
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
