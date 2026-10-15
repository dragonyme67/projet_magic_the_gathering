
#include <drogon/drogon.h>
#include <cstdlib>


int main() {
    const char* configFile = std::getenv("CONFIG_FILE");
    const char* listenAddr = std::getenv("LISTEN_ADDR");

    drogon::app().loadConfigFile(
        configFile ? configFile : "./config.json");

    drogon::app().registerHandler(
        "/api/cartes/test",
        [](const drogon::HttpRequestPtr&,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            auto db = drogon::app().getDbClient("default");
            db->execSqlAsync(
                "SELECT nom, rarete FROM Cartes LIMIT 5",

                // Requête réussie
                [callback](const drogon::orm::Result& result)
                {
                    Json::Value cartes(Json::arrayValue);

                    for (const auto& row : result)
                    {
                        Json::Value carte;
                        carte["nom"] = row["nom"].as<std::string>();
                        carte["rarete"] = row["rarete"].as<std::string>();

                        cartes.append(carte);
                    }

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(cartes);

                    callback(response);
                },

                // Erreur SQL ou connexion
                [callback](const drogon::orm::DrogonDbException& e)
                {
                    Json::Value erreur;
                    erreur["error"] = e.base().what();

                    auto response =
                        drogon::HttpResponse::newHttpJsonResponse(erreur);

                    response->setStatusCode(
                        drogon::k500InternalServerError
                    );

                    callback(response);
                }
            );
        }
    );
    drogon::app().addListener(listenAddr ? listenAddr : "127.0.0.1", 8080);
    drogon::app().run();
}
