#include "JeuController.h"

static drogon::HttpResponsePtr erreur(drogon::HttpStatusCode code, const std::string& message) {
    Json::Value json;
    json["error"] = message;
    auto reponse = drogon::HttpResponse::newHttpJsonResponse(json);
    reponse->setStatusCode(code);
    return reponse;
}

// détail dans les logs seulement
static drogon::HttpResponsePtr erreurInterne(const std::exception& e) {
    LOG_ERROR << e.what();
    return erreur(drogon::k500InternalServerError, "Erreur interne.");
}

// false si le champ est absent ou n'est pas un entier
static bool lireEntier(const drogon::HttpRequestPtr& req, const char* cle, int& valeur) {
    const std::shared_ptr<Json::Value> corps = req->getJsonObject();
    if (!corps || !corps->isObject()) return false;

    const Json::Value& json = *corps;
    if (!json[cle].isInt()) return false;
    valeur = json[cle].asInt();
    return true;
}

// false si le champ est absent ou n'est pas un texte
static bool lireTexte(const drogon::HttpRequestPtr& req, const char* cle, std::string& valeur) {
    const std::shared_ptr<Json::Value> corps = req->getJsonObject();
    if (!corps || !corps->isObject()) return false;

    const Json::Value& json = *corps;
    if (!json[cle].isString()) return false;
    valeur = json[cle].asString();
    return true;
}

void JeuController::creer(
    const drogon::HttpRequestPtr&,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    try {
        auto reponse = drogon::HttpResponse::newHttpJsonResponse(service_.creer());
        reponse->setStatusCode(drogon::k201Created);
        callback(reponse);
    } catch (const std::exception& e) {
        callback(erreurInterne(e));
    }
}

void JeuController::etat(
    const drogon::HttpRequestPtr&,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    try {
        Json::Value json;
        if (!service_.etat(json)) {
            callback(erreur(drogon::k404NotFound, "Aucune partie en cours."));
            return;
        }
        callback(drogon::HttpResponse::newHttpJsonResponse(json));
    } catch (const std::exception& e) {
        callback(erreurInterne(e));
    }
}

void JeuController::piocher(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    int joueur = 0;
    if (!lireEntier(req, "joueur", joueur)) {
        callback(erreur(drogon::k400BadRequest, "Corps attendu : {\"joueur\": <entier>}"));
        return;
    }
    callback(appliquer(joueur, magic::Action::piocher()));
}

void JeuController::deplacer(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    int joueur = 0;
    int objet = 0;
    std::string cle;
    magic::Zone zone = magic::Zone::Main;
    if (!lireEntier(req, "joueur", joueur) || !lireEntier(req, "objet", objet) ||
        !lireTexte(req, "zone", cle) || !zoneDepuisCle(cle, zone)) {
        callback(erreur(drogon::k400BadRequest,
                        "Corps attendu : {\"joueur\": <entier>, \"objet\": <entier>, "
                        "\"zone\": \"bibliotheque|main|champ_de_bataille|cimetiere|exil\"}"));
        return;
    }
    callback(appliquer(joueur, magic::Action::deplacer(objet, zone)));
}

void JeuController::engager(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    int joueur = 0;
    int objet = 0;
    if (!lireEntier(req, "joueur", joueur) || !lireEntier(req, "objet", objet)) {
        callback(erreur(drogon::k400BadRequest, "Corps attendu : {\"joueur\": <entier>, \"objet\": <entier>}"));
        return;
    }
    callback(appliquer(joueur, magic::Action::engager(objet)));
}

void JeuController::degager(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    int joueur = 0;
    int objet = 0;
    if (!lireEntier(req, "joueur", joueur) || !lireEntier(req, "objet", objet)) {
        callback(erreur(drogon::k400BadRequest, "Corps attendu : {\"joueur\": <entier>, \"objet\": <entier>}"));
        return;
    }
    callback(appliquer(joueur, magic::Action::degager(objet)));
}

void JeuController::changerVie(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    int joueur = 0;
    int cible = 0;
    int delta = 0;
    if (!lireEntier(req, "joueur", joueur) || !lireEntier(req, "cible", cible) ||
        !lireEntier(req, "delta", delta)) {
        callback(erreur(drogon::k400BadRequest,
                        "Corps attendu : {\"joueur\": <entier>, \"cible\": <entier>, \"delta\": <entier>}"));
        return;
    }
    callback(appliquer(joueur, magic::Action::changerVie(cible, delta)));
}

void JeuController::finDuTour(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    int joueur = 0;
    if (!lireEntier(req, "joueur", joueur)) {
        callback(erreur(drogon::k400BadRequest, "Corps attendu : {\"joueur\": <entier>}"));
        return;
    }
    callback(appliquer(joueur, magic::Action::finDuTour()));
}

drogon::HttpResponsePtr JeuController::appliquer(magic::JoueurId joueur, const magic::Action& action) {
    try {
        ReponseAction reponse;
        if (!service_.appliquer(joueur, action, reponse)) {
            return erreur(drogon::k404NotFound, "Aucune partie en cours.");
        }
        auto http = drogon::HttpResponse::newHttpJsonResponse(reponse.json);
        if (!reponse.accepte) http->setStatusCode(drogon::k409Conflict);
        return http;
    } catch (const std::exception& e) {
        return erreurInterne(e);
    }
}
