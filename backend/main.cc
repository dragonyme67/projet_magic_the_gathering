#include <drogon/drogon.h>


int main() {
    drogon::app().loadConfigFile("/home/etud/Documents/Projet_Magic/backend/config.json");

    drogon::app().registerBeginningAdvice([] {
        auto db = drogon::app().getDbClient("default");
        // test : db->execSqlAsync(...)
    });

    drogon::app().registerHandler("/api/hello",
        [](const drogon::HttpRequestPtr&,
           std::function<void(const drogon::HttpResponsePtr&)>&& cb) {
            Json::Value j; j["message"] = "Salut depuis Drogon";
            cb(drogon::HttpResponse::newHttpJsonResponse(j));
        }, {drogon::Get});

    drogon::app().addListener("127.0.0.1", 8080);
    drogon::app().run();
}