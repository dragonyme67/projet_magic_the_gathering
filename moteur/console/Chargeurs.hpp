#pragma once

#include <string>
#include <vector>

#include "magic/Catalogue.hpp"

magic::Catalogue chargerCatalogue(const std::string& baseSqlite);

// Format : { "joueurs": [ { "nom": "Alice", "cartes": [ { "id": "<id table Cartes>", "quantite": 4 } ] }, { ... } ] }
// quantite = 1 par défaut, autres clés ignorées
std::vector<magic::Deck> chargerDecks(const std::string& fichierJson, const magic::Catalogue& catalogue);
