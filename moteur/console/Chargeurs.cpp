#include "Chargeurs.hpp"

#include <json/json.h>
#include <sqlite3.h>

#include <fstream>
#include <stdexcept>

namespace {

// Texte d'une colonne de la requête ; chaîne vide si la valeur est NULL.
std::string colonne(sqlite3_stmt* requete, int numero) {
    const unsigned char* texte = sqlite3_column_text(requete, numero);
    if (texte == nullptr) return "";
    return reinterpret_cast<const char*>(texte);
}

}  // namespace

magic::Catalogue chargerCatalogue(const std::string& baseSqlite) {
    // Ouverture en lecture seule : si le fichier n'existe pas, on a une erreur
    // (sinon SQLite créerait une base vide).
    sqlite3* base = nullptr;
    if (sqlite3_open_v2(baseSqlite.c_str(), &base, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK) {
        const std::string message = "Impossible d'ouvrir la base " + baseSqlite + " : " + sqlite3_errmsg(base);
        sqlite3_close(base);
        throw std::runtime_error(message);
    }

    const char* sql =
        "SELECT id, oracle_id, nom, nom_imprime, cout_mana, ligne_type, ligne_type_imprimee,"
        "       texte_oracle, texte_imprime, force, endurance FROM Cartes";
    sqlite3_stmt* requete = nullptr;
    if (sqlite3_prepare_v2(base, sql, -1, &requete, nullptr) != SQLITE_OK) {
        const std::string message = std::string("Lecture de la table Cartes impossible : ") + sqlite3_errmsg(base);
        sqlite3_close(base);
        throw std::runtime_error(message);
    }

    // Une ligne de la table = une carte.
    magic::Catalogue catalogue;
    while (sqlite3_step(requete) == SQLITE_ROW) {
        magic::Carte carte;
        carte.id = colonne(requete, 0);
        carte.oracleId = colonne(requete, 1);
        carte.nom = colonne(requete, 2);
        carte.nomImprime = colonne(requete, 3);
        carte.coutMana = colonne(requete, 4);
        carte.ligneType = colonne(requete, 5);
        carte.ligneTypeImprimee = colonne(requete, 6);
        carte.texteOracle = colonne(requete, 7);
        carte.texteImprime = colonne(requete, 8);
        carte.force = colonne(requete, 9);
        carte.endurance = colonne(requete, 10);
        catalogue.ajouter(carte);
    }

    sqlite3_finalize(requete);
    sqlite3_close(base);
    return catalogue;
}

std::vector<magic::Deck> chargerDecks(const std::string& fichierJson, const magic::Catalogue& catalogue) {
    std::ifstream flux(fichierJson);
    if (!flux) throw std::runtime_error("Impossible de lire le fichier de decks " + fichierJson);

    Json::Value lue;
    std::string erreurs;
    if (!Json::parseFromStream(Json::CharReaderBuilder(), flux, &lue, &erreurs)) {
        throw std::runtime_error(fichierJson + " : JSON invalide : " + erreurs);
    }

    const Json::Value& racine = lue;  // lecture seule : operator[] ne crée aucune clé
    if (!racine.isObject() || !racine["joueurs"].isArray() || racine["joueurs"].size() != 2) {
        throw std::runtime_error(fichierJson + " : \"joueurs\" doit être un tableau de exactement 2 joueurs.");
    }

    std::vector<magic::Deck> decks;
    for (const Json::Value& joueur : racine["joueurs"]) {
        if (!joueur.isObject() || !joueur["nom"].isString() || !joueur["cartes"].isArray()) {
            throw std::runtime_error(fichierJson + " : chaque joueur doit avoir un \"nom\" et un tableau \"cartes\".");
        }
        const std::string nom = joueur["nom"].asString();

        std::vector<magic::LigneDeck> lignes;
        for (const Json::Value& carte : joueur["cartes"]) {
            if (!carte.isObject() || !carte["id"].isString()) {
                throw std::runtime_error(fichierJson + " : deck de " + nom + " : chaque carte doit avoir un \"id\" (texte).");
            }
            const Json::Value& quantite = carte["quantite"];
            if (!quantite.isNull() && !quantite.isInt()) {
                throw std::runtime_error(fichierJson + " : deck de " + nom + " : \"quantite\" doit être un entier.");
            }
            lignes.push_back({carte["id"].asString(), quantite.isNull() ? 1 : quantite.asInt()});
        }
        decks.push_back(magic::construireDeck(catalogue, nom, lignes));
    }
    return decks;
}
