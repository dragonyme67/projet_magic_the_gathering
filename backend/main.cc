
#include <drogon/drogon.h>
#include <cstdlib>


int main() {
    const char* configFile = std::getenv("CONFIG_FILE");
    const char* listenAddr = std::getenv("LISTEN_ADDR");
    drogon::app().loadConfigFile(
        configFile ? configFile : "./config.json");
    drogon::app().addListener(listenAddr ? listenAddr : "127.0.0.1", 8080);
    drogon::app().run();
}

