
#include <drogon/drogon.h>
#include <cstdlib>


int main() {
    const char* configFile = std::getenv("CONFIG_FILE");
    const char* listenAddr = std::getenv("LISTEN_ADDR");

    drogon::app().loadConfigFile(
        configFile ? configFile : "/home/etud/Documents/Projet_Magic/backend/config.json");

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
    drogon::app().addListener(listenAddr ? listenAddr : "127.0.0.1", 8080);
    drogon::app().run();
}
