#pragma once

#include <drogon/HttpController.h>

class CarteController : public drogon::HttpController<CarteController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CarteController::test, "/api/cartes/test", drogon::Get);
    METHOD_LIST_END

    void test(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    );
};
