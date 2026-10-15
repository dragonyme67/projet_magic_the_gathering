#include "HelloController.h"

void HelloController::hello(
    const drogon::HttpRequestPtr&,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    Json::Value json;
    json["message"] = "Bonjour depuis Drogon !";

    auto response = drogon::HttpResponse::newHttpJsonResponse(json);
    callback(response);
}