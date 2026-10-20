#pragma once

#include <drogon/HttpController.h>

#include "services/JeuService.h"

class JeuController : public drogon::HttpController<JeuController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(JeuController::creer, "/api/jeu", drogon::Post);
    ADD_METHOD_TO(JeuController::etat, "/api/jeu", drogon::Get);
    ADD_METHOD_TO(JeuController::piocher, "/api/jeu/piocher", drogon::Post);
    ADD_METHOD_TO(JeuController::deplacer, "/api/jeu/deplacer", drogon::Post);
    ADD_METHOD_TO(JeuController::engager, "/api/jeu/engager", drogon::Post);
    ADD_METHOD_TO(JeuController::degager, "/api/jeu/degager", drogon::Post);
    ADD_METHOD_TO(JeuController::changerVie, "/api/jeu/vie", drogon::Post);
    ADD_METHOD_TO(JeuController::finDuTour, "/api/jeu/fin-du-tour", drogon::Post);
    METHOD_LIST_END

    void creer( const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void etat( const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void piocher( const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void deplacer( const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void engager( const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void degager( const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void changerVie( const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void finDuTour(const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

private:
    drogon::HttpResponsePtr appliquer(magic::JoueurId joueur, const magic::Action &action);

    JeuService service_;
};
