
#include <drogon/drogon.h>


int main() {
    drogon::app().loadConfigFile("/home/etud/Documents/Projet_Magic/backend/config.json");

    drogon::app().registerBeginningAdvice([] {
        auto db = drogon::app().getDbClient("default");
        // test : db->execSqlAsync(...)
    });

    drogon::app().registerHandler(
        "/api/hello",
        [](const drogon::HttpRequestPtr&,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
            Json::Value json;
            json["message"] = "Bonjour depuis Drogon !";

            auto response = drogon::HttpResponse::newHttpJsonResponse(json);
            callback(response);
        }
    );
    drogon::app().addListener("127.0.0.1", 8080);
    drogon::app().run();
}
