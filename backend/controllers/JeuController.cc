#include "JeuController.h"

namespace {

drogon::HttpResponsePtr erreur(drogon::HttpStatusCode code, const std::string& message) {
    Json::Value json;
    json["error"] = message;
    auto reponse = drogon::HttpResponse::newHttpJsonResponse(json);
    reponse->setStatusCode(code);
    return reponse;
}

// détail dans les logs seulement
drogon::HttpResponsePtr erreurInterne(const std::exception& e) {
    LOG_ERROR << e.what();
    return erreur(drogon::k500InternalServerError, "Erreur interne.");
}

}

void JeuController::creer(const drogon::HttpRequestPtr&, Callback&& callback) {
    try {
        auto reponse = drogon::HttpResponse::newHttpJsonResponse(service_.creer());
        reponse->setStatusCode(drogon::k201Created);
        callback(reponse);
    } catch (const std::exception& e) {
        callback(erreurInterne(e));
    }
}

void JeuController::etat(const drogon::HttpRequestPtr&, Callback&& callback) {
    try {
        const std::optional<Json::Value> etat = service_.etat();
        if (!etat) {
            callback(erreur(drogon::k404NotFound, "Aucune partie en cours."));
            return;
        }
        callback(drogon::HttpResponse::newHttpJsonResponse(*etat));
    } catch (const std::exception& e) {
        callback(erreurInterne(e));
    }
}
