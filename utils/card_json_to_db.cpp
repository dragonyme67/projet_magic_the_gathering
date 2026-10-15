
#include <sqlite3.h>
#include <json/json.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

// g++ utils/card_json_to_db.cpp -o utils/card_json_to_db \
    $(pkg-config --cflags --libs sqlite3 jsoncpp)

// Retourne une chaîne vide si le champ est absent ou null.
std::string getString(const Json::Value& obj,
                      const std::string& key)
{
    if (!obj.isMember(key) || obj[key].isNull())
        return "";

    return obj[key].asString();
}

// Sérialise un tableau ou objet JSON en texte.
std::string toJson(const Json::Value& value)
{
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";

    return Json::writeString(builder, value);
}

// Lie une chaîne à un paramètre SQL nommé.
void bindText(sqlite3* db,
              sqlite3_stmt* stmt,
              const char* parameter,
              const std::string& value)
{
    int index = sqlite3_bind_parameter_index(stmt, parameter);

    if (index == 0 ||
        sqlite3_bind_text(stmt, index, value.c_str(),
                          -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
}

// Lie un champ nullable : SQL NULL si absent.
void bindNullableText(sqlite3* db,
                      sqlite3_stmt* stmt,
                      const char* parameter,
                      const Json::Value& obj,
                      const char* key)
{
    int index = sqlite3_bind_parameter_index(stmt, parameter);

    if (index == 0)
        throw std::runtime_error("Parametre SQL inconnu");

    if (!obj.isMember(key) || obj[key].isNull())
    {
        sqlite3_bind_null(stmt, index);
    }
    else
    {
        std::string value = obj[key].asString();

        if (sqlite3_bind_text(stmt, index, value.c_str(),
                              -1, SQLITE_TRANSIENT) != SQLITE_OK)
            throw std::runtime_error(sqlite3_errmsg(db));
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Utilisation : " << argv[0]
                  << " <fichier_json>\n";
        std::cerr << "Exemple : " << argv[0]
                  << " resources/kld.json\n";
        return 1;
    }

    // Chemin du JSON donné en argument
    std::filesystem::path jsonPath = argv[1];
    std::ifstream file(jsonPath);

    if (!file)
    {
        std::cerr << "Impossible d'ouvrir le fichier JSON\n";
        return 1;
    }

    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errors;

    if (!Json::parseFromStream(builder, file, &root, &errors))
    {
        std::cerr << "JSON invalide : " << errors << '\n';
        return 1;
    }

    if (!root.isArray())
    {
        std::cerr << "Le JSON doit contenir un tableau de cartes\n";
        return 1;
    }

    // 2. Ouvrir la base SQLite existante
    sqlite3* db = nullptr;

    if (sqlite3_open("data/database.sqlite", &db) != SQLITE_OK)
    {
        std::cerr << "Erreur SQLite : "
                  << sqlite3_errmsg(db) << '\n';

        if (db) sqlite3_close(db);
        return 1;
    }

    // 3. Préparer une seule requête pour toutes les cartes
    const char* sql = R"SQL(
        INSERT OR IGNORE INTO Cartes (
            id, oracle_id, multiverse_id,
            nom, nom_imprime, langue, date_sortie, layout,
            cout_mana, cmc, ligne_type, ligne_type_imprimee,
            texte_oracle, texte_imprime, texte_ambiance,
            force, endurance, loyaute,
            couleurs, identite_couleur, mots_cles, legalites,
            code_set, nom_set, numero_collection, rarete, artiste,
            image_small, image_normal, image_large, image_png,
            image_art_crop, uri_scryfall, uri_rulings, edhrec_rank
        ) VALUES (
            :id, :oracle_id, :multiverse_id,
            :nom, :nom_imprime, :langue, :date_sortie, :layout,
            :cout_mana, :cmc, :ligne_type, :ligne_type_imprimee,
            :texte_oracle, :texte_imprime, :texte_ambiance,
            :force, :endurance, :loyaute,
            :couleurs, :identite_couleur, :mots_cles, :legalites,
            :code_set, :nom_set, :numero_collection, :rarete, :artiste,
            :image_small, :image_normal, :image_large, :image_png,
            :image_art_crop, :uri_scryfall, :uri_rulings, :edhrec_rank
        );
    )SQL";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Erreur SQL : " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return 1;
    }

    int imported = 0;
    int ignored = 0;

    try
    {
        // 4. Importer dans une transaction
        char* error = nullptr;

        if (sqlite3_exec(db, "BEGIN", nullptr, nullptr, &error) != SQLITE_OK)
        {
            std::string message = error ? error : "Erreur transaction";
            sqlite3_free(error);
            throw std::runtime_error(message);
        }

        for (const auto& card : root)
        {
            // Vérification minimale des champs obligatoires
            if (getString(card, "id").empty() ||
                getString(card, "oracle_id").empty() ||
                getString(card, "name").empty() ||
                getString(card, "type_line").empty() ||
                getString(card, "set").empty() ||
                getString(card, "collector_number").empty() ||
                getString(card, "rarity").empty())
            {
                ++ignored;
                continue;
            }

            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);

            auto text = [&](const char* parameter,
                            const std::string& value)
            {
                bindText(db, stmt, parameter, value);
            };

            auto nullable = [&](const char* parameter,
                                const char* key)
            {
                bindNullableText(db, stmt, parameter, card, key);
            };

            const Json::Value& images = card["image_uris"];

            // Identifiants
            text(":id", getString(card, "id"));
            text(":oracle_id", getString(card, "oracle_id"));

            if (card["multiverse_ids"].isArray() &&
                !card["multiverse_ids"].empty() &&
                !card["multiverse_ids"][0].isNull())
            {
                sqlite3_bind_int(
                    stmt,
                    sqlite3_bind_parameter_index(stmt, ":multiverse_id"),
                    card["multiverse_ids"][0].asInt()
                );
            }
            else
            {
                sqlite3_bind_null(
                    stmt,
                    sqlite3_bind_parameter_index(stmt, ":multiverse_id")
                );
            }

            // Informations générales
            text(":nom", getString(card, "name"));
            nullable(":nom_imprime", "printed_name");
            text(":langue", getString(card, "lang").empty()
                            ? "en" : getString(card, "lang"));
            nullable(":date_sortie", "released_at");
            text(":layout", getString(card, "layout").empty()
                             ? "normal" : getString(card, "layout"));
            nullable(":cout_mana", "mana_cost");

            sqlite3_bind_double(
                stmt,
                sqlite3_bind_parameter_index(stmt, ":cmc"),
                card.get("cmc", 0.0).asDouble()
            );

            text(":ligne_type", getString(card, "type_line"));
            nullable(":ligne_type_imprimee", "printed_type_line");
            nullable(":texte_oracle", "oracle_text");
            nullable(":texte_imprime", "printed_text");
            nullable(":texte_ambiance", "flavor_text");
            nullable(":force", "power");
            nullable(":endurance", "toughness");
            nullable(":loyaute", "loyalty");

            // Tableaux et objets conservés en JSON
            text(":couleurs", toJson(card.get("colors", Json::Value(Json::arrayValue))));
            text(":identite_couleur", toJson(card.get("color_identity", Json::Value(Json::arrayValue))));
            text(":mots_cles", toJson(card.get("keywords", Json::Value(Json::arrayValue))));
            text(":legalites", toJson(card.get("legalities", Json::Value(Json::objectValue))));

            // Extension
            text(":code_set", getString(card, "set"));
            nullable(":nom_set", "set_name");
            text(":numero_collection", getString(card, "collector_number"));
            text(":rarete", getString(card, "rarity"));
            nullable(":artiste", "artist");

            // Images
            for (const auto& entry : {
                std::pair<const char*, const char*>{":image_small", "small"},
                {":image_normal", "normal"},
                {":image_large", "large"},
                {":image_png", "png"},
                {":image_art_crop", "art_crop"}
            })
            {
                std::string value = getString(images, entry.second);

                if (value.empty())
                {
                    sqlite3_bind_null(
                        stmt,
                        sqlite3_bind_parameter_index(stmt, entry.first)
                    );
                }
                else
                {
                    text(entry.first, value);
                }
            }

            // URLs
            nullable(":uri_scryfall", "scryfall_uri");
            nullable(":uri_rulings", "rulings_uri");

            // Classement EDHREC : NULL s'il est absent
            if (card.isMember("edhrec_rank") &&
                !card["edhrec_rank"].isNull())
            {
                sqlite3_bind_int(
                    stmt,
                    sqlite3_bind_parameter_index(stmt, ":edhrec_rank"),
                    card["edhrec_rank"].asInt()
                );
            }
            else
            {
                sqlite3_bind_null(
                    stmt,
                    sqlite3_bind_parameter_index(stmt, ":edhrec_rank")
                );
            }

            if (sqlite3_step(stmt) != SQLITE_DONE)
                throw std::runtime_error(sqlite3_errmsg(db));

            if (sqlite3_changes(db) == 1)
                ++imported;
            else
                ++ignored;
        }

        if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, &error) != SQLITE_OK)
        {
            std::string message = error ? error : "Erreur commit";
            sqlite3_free(error);
            throw std::runtime_error(message);
        }
    }
    catch (const std::exception& e)
    {
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        std::cerr << "Import annule : " << e.what() << '\n';

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "Import termine !\n"
              << "Cartes ajoutees : " << imported << '\n'
              << "Cartes ignorees : " << ignored << '\n';

    return 0;
}
