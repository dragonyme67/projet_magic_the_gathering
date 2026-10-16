#pragma once

#include <string>
#include <vector>

#include "magic/Catalogue.hpp"

// Lit toutes les cartes de la DB Cartes.
magic::Catalogue chargerCatalogue(const std::string& baseSqlite);

// Lit le fichier de decks et construit leurs decks. Format :
//   { "joueurs": [ { "nom": "Alice", "cartes": [ { "id": "<id table Cartes>", "quantite": 4 } ] },
//                  { "nom": "Bob",   "cartes": [ ... ] } ] }
// `quantite` vaut 1 par défaut ; les autres clés (ex. "nom" d'une carte) sont ignorées.
std::vector<magic::Deck> chargerDecks(const std::string& fichierJson, const magic::Catalogue& catalogue);
