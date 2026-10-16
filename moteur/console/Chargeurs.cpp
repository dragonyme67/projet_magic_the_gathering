#include "Chargeurs.hpp"

#include <json/json.h>
#include <sqlite3.h>

#include <fstream>
#include <memory>
#include <stdexcept>

namespace {

struct FermerBase { void operator()(sqlite3* db) const { sqlite3_close(db); } };
struct FinaliserRequete { void operator()(sqlite3_stmt* s) const { sqlite3_finalize(s); } };

// Texte d'une colonne ; chaîne vide si NULL.
std::string colonne(sqlite3_stmt* requete, int i) {
    const unsigned char* texte = sqlite3_column_text(requete, i);
    return texte ? reinterpret_cast<const char*>(texte) : "";
}

}  // namespace

magic::Catalogue chargerCatalogue(const std::string& baseSqlite) {
    sqlite3* brut = nullptr;
    const int ouverture = sqlite3_open_v2(baseSqlite.c_str(), &brut, SQLITE_OPEN_READONLY, nullptr);
    std::unique_ptr<sqlite3, FermerBase> base(brut);
    if (ouverture != SQLITE_OK) {
        throw std::runtime_error("Impossible d'ouvrir la base " + baseSqlite + " : " + sqlite3_errmsg(brut));
    }

    const char* sql =
        "SELECT id, oracle_id, nom, nom_imprime, cout_mana, ligne_type, ligne_type_imprimee,"
        "       texte_oracle, texte_imprime, force, endurance FROM Cartes";
    sqlite3_stmt* requeteBrute = nullptr;
    if (sqlite3_prepare_v2(base.get(), sql, -1, &requeteBrute, nullptr) != SQLITE_OK) {
        throw std::runtime_error(std::string("Lecture de la table Cartes impossible : ") +
                                 sqlite3_errmsg(base.get()));
    }
    std::unique_ptr<sqlite3_stmt, FinaliserRequete> requete(requeteBrute);

    magic::Catalogue catalogue;
    while (sqlite3_step(requete.get()) == SQLITE_ROW) {
        magic::Carte carte;
        carte.id = colonne(requete.get(), 0);
        carte.oracleId = colonne(requete.get(), 1);
        carte.nom = colonne(requete.get(), 2);
        carte.nomImprime = colonne(requete.get(), 3);
        carte.coutMana = colonne(requete.get(), 4);
        carte.ligneType = colonne(requete.get(), 5);
        carte.ligneTypeImprimee = colonne(requete.get(), 6);
        carte.texteOracle = colonne(requete.get(), 7);
        carte.texteImprime = colonne(requete.get(), 8);
        carte.force = colonne(requete.get(), 9);
        carte.endurance = colonne(requete.get(), 10);
        catalogue.ajouter(std::move(carte));
    }
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
