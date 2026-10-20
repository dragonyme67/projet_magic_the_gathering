#include "magic/io/DeckRepository.hpp"

#include <json/json.h>
#include <sqlite3.h>

#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace magic {

namespace {

struct Ligne {
    std::string id;
    int quantite;
};

using Base = std::unique_ptr<sqlite3, int (*)(sqlite3*)>;
using Requete = std::unique_ptr<sqlite3_stmt, int (*)(sqlite3_stmt*)>;

// "" si NULL
std::string colonne(sqlite3_stmt* requete, int numero) {
    const unsigned char* texte = sqlite3_column_text(requete, numero);
    return texte == nullptr ? "" : reinterpret_cast<const char*>(texte);
}

// jamais de chemin
bool identifiantValide(const std::string& id) {
    if (id.empty() || id.size() > 64) return false;
    for (char c : id) {
        const bool ok = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
        if (!ok) return false;
    }
    return true;
}

std::vector<Ligne> lireLignes(const std::string& fichier, const std::string& id) {
    std::ifstream flux(fichier);
    if (!flux) throw std::invalid_argument("Deck inconnu : " + id);

    Json::Value racine;
    std::string erreurs;
    if (!Json::parseFromStream(Json::CharReaderBuilder(), flux, &racine, &erreurs)) {
        throw std::runtime_error(fichier + " : JSON invalide : " + erreurs);
    }
    if (!racine.isObject() || !racine["cartes"].isArray()) {
        throw std::runtime_error(fichier + " : \"cartes\" doit être un tableau.");
    }

    std::vector<Ligne> lignes;
    for (const Json::Value& carte : racine["cartes"]) {
        if (!carte.isObject() || !carte["id"].isString()) {
            throw std::runtime_error(fichier + " : chaque carte doit avoir un \"id\" (texte).");
        }
        const Json::Value& quantite = carte["quantite"];
        if (!quantite.isNull() && !quantite.isInt()) {
            throw std::runtime_error(fichier + " : \"quantite\" doit être un entier.");
        }
        const int n = quantite.isNull() ? 1 : quantite.asInt();
        if (n < 1) throw std::runtime_error(fichier + " : \"quantite\" doit être >= 1.");
        lignes.push_back({carte["id"].asString(), n});
    }
    if (lignes.empty()) throw std::runtime_error(fichier + " : deck vide.");
    return lignes;
}

std::vector<Carte> lireCartes(const std::string& baseSqlite, const std::vector<Ligne>& lignes,
                              const std::string& idDeck) {
    sqlite3* brut = nullptr;
    const int code = sqlite3_open_v2(baseSqlite.c_str(), &brut, SQLITE_OPEN_READONLY, nullptr);
    Base base(brut, &sqlite3_close);
    if (code != SQLITE_OK) {
        throw std::runtime_error("Impossible d'ouvrir la base " + baseSqlite + " : " + sqlite3_errmsg(base.get()));
    }

    std::string sql =
        "SELECT id, oracle_id, nom, nom_imprime, cout_mana, ligne_type, ligne_type_imprimee,"
        " texte_oracle, texte_imprime, force, endurance, image_png FROM Cartes WHERE id IN (";
    for (size_t i = 0; i < lignes.size(); ++i) sql += (i == 0 ? "?" : ",?");
    sql += ")";

    sqlite3_stmt* brutRequete = nullptr;
    if (sqlite3_prepare_v2(base.get(), sql.c_str(), -1, &brutRequete, nullptr) != SQLITE_OK) {
        throw std::runtime_error(std::string("Lecture de la table Cartes impossible : ") + sqlite3_errmsg(base.get()));
    }
    Requete requete(brutRequete, &sqlite3_finalize);

    for (size_t i = 0; i < lignes.size(); ++i) {
        sqlite3_bind_text(requete.get(), static_cast<int>(i) + 1, lignes[i].id.c_str(), -1, SQLITE_TRANSIENT);
    }

    std::unordered_map<std::string, Carte> parId;
    int etat;
    while ((etat = sqlite3_step(requete.get())) == SQLITE_ROW) {
        Carte carte;
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
        carte.image = colonne(requete.get(), 11);
        parId[carte.id] = carte;
    }
    if (etat != SQLITE_DONE) {
        throw std::runtime_error(std::string("Lecture des cartes interrompue : ") + sqlite3_errmsg(base.get()));
    }

    std::vector<Carte> cartes;
    for (const Ligne& ligne : lignes) {
        auto it = parId.find(ligne.id);
        if (it == parId.end()) {
            throw std::runtime_error("Deck " + idDeck + " : carte absente de la base (id " + ligne.id + ").");
        }
        cartes.insert(cartes.end(), ligne.quantite, it->second);
    }
    return cartes;
}

}

DeckRepository::DeckRepository(std::string dossierDecks, std::string baseSqlite)
    : dossier_(std::move(dossierDecks)), base_(std::move(baseSqlite)) {}

Deck DeckRepository::charger(const std::string& id, const std::string& joueur) {
    if (!identifiantValide(id)) throw std::invalid_argument("Identifiant de deck invalide : " + id);

    auto it = cache_.find(id);
    if (it == cache_.end()) {
        const std::vector<Ligne> lignes = lireLignes(dossier_ + "/" + id + ".json", id);
        it = cache_.emplace(id, lireCartes(base_, lignes, id)).first;
    }
    return Deck{joueur, it->second};
}

}
