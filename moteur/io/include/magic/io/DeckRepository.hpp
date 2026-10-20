#pragma once

#include <map>
#include <string>
#include <vector>

#include "magic/Carte.hpp"

namespace magic {

// Decks JSON + cartes SQLite, mis en cache. Pas thread-safe
class DeckRepository {
public:
    DeckRepository(std::string dossierDecks, std::string baseSqlite);

    // invalid_argument : id invalide ou deck inconnu ; runtime_error : lecture impossible
    Deck charger(const std::string& id, const std::string& joueur);

private:
    std::string dossier_;
    std::string base_;
    std::map<std::string, std::vector<Carte>> cache_;
};

}
