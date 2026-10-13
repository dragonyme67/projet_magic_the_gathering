#pragma once

#include <memory>
#include <string>
#include <vector>

#include <json/json.h>

#include "cartes/Carte.hpp"

// Conversion du JSON Scryfall en cartes.
namespace parser {

// Lit et parse un fichier JSON. Lève std::runtime_error si le fichier est illisible ou invalide.
Json::Value lireFichierJson(const std::string& chemin);

// Construit la carte décrite par un objet JSON Scryfall.
// La classe fille est choisie selon le type principal ; nullptr si le type n'est pas géré.
std::unique_ptr<Carte> parserCarte(const Json::Value& json);

// Charge toutes les cartes d'un fichier Scryfall (tableau JSON).
// Les cartes d'un type non géré sont ignorées avec un avertissement.
std::vector<std::unique_ptr<Carte>> chargerCartes(const std::string& chemin);

}  // namespace parser
