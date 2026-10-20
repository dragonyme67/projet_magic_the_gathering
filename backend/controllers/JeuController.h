#pragma once

#include <drogon/HttpController.h>

#include "services/JeuService.h"

class JeuController : public drogon::HttpController<JeuController> {
public:
    using Callback = std::function<void(const drogon::HttpResponsePtr&)>;

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(JeuController::creer, "/api/jeu", drogon::Post);
    ADD_METHOD_TO(JeuController::etat, "/api/jeu", drogon::Get);
    METHOD_LIST_END

    void creer(const drogon::HttpRequestPtr& req, Callback&& callback);
    void etat(const drogon::HttpRequestPtr& req, Callback&& callback);

private:
    JeuService service_;
};
