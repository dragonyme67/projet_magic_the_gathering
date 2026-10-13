#include "parser/ParserCartes.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "cartes/Artefact.hpp"
#include "cartes/Creature.hpp"
#include "cartes/Enchantement.hpp"
#include "cartes/Ephemere.hpp"
#include "cartes/Planeswalker.hpp"
#include "cartes/Rituel.hpp"
#include "cartes/Terrain.hpp"

namespace parser {

namespace {

// Sépare types et sous-types dans "type_line" : tiret cadratin UTF-8 entouré d'espaces.
const std::string SEPARATEUR_SOUS_TYPES = " — ";

const std::vector<std::string> SUPERTYPES = {"Basic", "Legendary", "Snow", "World", "Ongoing"};

bool contient(const std::vector<std::string>& liste, const std::string& valeur) {
    return std::find(liste.begin(), liste.end(), valeur) != liste.end();
}

std::string lireTexte(const Json::Value& json, const char* cle) {
    return json.get(cle, "").asString();
}

// Scryfall stocke force, endurance et loyauté en texte ; une valeur variable ("*") vaut 0.
int lireEntier(const Json::Value& json, const char* cle) {
    try {
        return std::stoi(lireTexte(json, cle));
    } catch (const std::exception&) {
        return 0;
    }
}

std::vector<std::string> lireListe(const Json::Value& json, const char* cle) {
    std::vector<std::string> liste;
    for (const auto& element : json[cle]) {
        liste.push_back(element.asString());
    }
    return liste;
}

ForceEndurance lireForceEndurance(const Json::Value& json) {
    return {lireEntier(json, "power"), lireEntier(json, "toughness")};
}

std::vector<std::string> decouperMots(const std::string& texte) {
    std::istringstream flux(texte);
    std::vector<std::string> mots;
    for (std::string mot; flux >> mot;) {
        mots.push_back(mot);
    }
    return mots;
}

// "Legendary Artifact Creature — Human Artificer"
//   -> supertypes {Legendary}, types {Artifact, Creature}, sous-types {Human, Artificer}
void decouperLigneType(const std::string& ligne, DonneesCarte& donnees) {
    const auto separateur = ligne.find(SEPARATEUR_SOUS_TYPES);

    for (auto& mot : decouperMots(ligne.substr(0, separateur))) {
        auto& destination = contient(SUPERTYPES, mot) ? donnees.supertypes : donnees.types;
        destination.push_back(std::move(mot));
    }
    if (separateur != std::string::npos) {
        donnees.sousTypes = decouperMots(ligne.substr(separateur + SEPARATEUR_SOUS_TYPES.size()));
    }
}

DonneesCarte lireDonneesCommunes(const Json::Value& json) {
    DonneesCarte donnees;
    donnees.id = lireTexte(json, "id");
    donnees.nom = lireTexte(json, "name");
    donnees.nomFr = lireTexte(json, "printed_name");
    donnees.coutMana = lireTexte(json, "mana_cost");
    donnees.valeurMana = json.get("cmc", 0).asInt();
    donnees.couleurs = lireListe(json, "colors");
    donnees.motsCles = lireListe(json, "keywords");
    donnees.ligneTypeFr = lireTexte(json, "printed_type_line");
    donnees.texte = lireTexte(json, "oracle_text");
    donnees.texteFr = lireTexte(json, "printed_text");
    decouperLigneType(lireTexte(json, "type_line"), donnees);
    return donnees;
}

}  // namespace

Json::Value lireFichierJson(const std::string& chemin) {
    std::ifstream fichier(chemin);
    if (!fichier) {
        throw std::runtime_error("Impossible d'ouvrir " + chemin);
    }

    Json::CharReaderBuilder lecteur;
    Json::Value racine;
    std::string erreurs;
    if (!Json::parseFromStream(lecteur, fichier, &racine, &erreurs)) {
        throw std::runtime_error("JSON invalide dans " + chemin + " : " + erreurs);
    }
    return racine;
}

std::unique_ptr<Carte> parserCarte(const Json::Value& json) {
    DonneesCarte donnees = lireDonneesCommunes(json);
    const auto estDeType = [&donnees](const char* type) { return contient(donnees.types, type); };

    // L'ordre des tests fixe la priorité : une "Artifact Creature" devient une Creature.
    if (estDeType("Creature")) {
        return std::make_unique<Creature>(std::move(donnees), lireForceEndurance(json));
    }
    if (estDeType("Planeswalker")) {
        return std::make_unique<Planeswalker>(std::move(donnees), lireEntier(json, "loyalty"));
    }
    if (estDeType("Land")) {
        return std::make_unique<Terrain>(std::move(donnees), lireListe(json, "produced_mana"));
    }
    if (estDeType("Artifact")) {
        std::optional<ForceEndurance> forceEndurance;
        if (json.isMember("power")) {
            forceEndurance = lireForceEndurance(json);
        }
        return std::make_unique<Artefact>(std::move(donnees), forceEndurance);
    }
    if (estDeType("Enchantment")) {
        return std::make_unique<Enchantement>(std::move(donnees));
    }
    if (estDeType("Instant")) {
        return std::make_unique<Ephemere>(std::move(donnees));
    }
    if (estDeType("Sorcery")) {
        return std::make_unique<Rituel>(std::move(donnees));
    }
    return nullptr;
}

std::vector<std::unique_ptr<Carte>> chargerCartes(const std::string& chemin) {
    const Json::Value racine = lireFichierJson(chemin);
    if (!racine.isArray()) {
        throw std::runtime_error(chemin + " : un tableau de cartes est attendu");
    }

    std::vector<std::unique_ptr<Carte>> cartes;
    cartes.reserve(racine.size());
    for (const auto& json : racine) {
        if (auto carte = parserCarte(json)) {
            cartes.push_back(std::move(carte));
        } else {
            std::cerr << "Type non géré, carte ignorée : " << lireTexte(json, "name")
                      << " (" << lireTexte(json, "type_line") << ")\n";
        }
    }
    return cartes;
}

}  // namespace parser
