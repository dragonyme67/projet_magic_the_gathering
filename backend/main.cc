#include <drogon/drogon.h>

int main() {
    drogon::app().registerHandler(
        "/test",
        [](const drogon::HttpRequestPtr&,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setBody("Drogon fonctionne !");
            callback(response);
        }
    );

    drogon::app().addListener("127.0.0.1", 8080);
    drogon::app().run();
}