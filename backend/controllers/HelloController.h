
#pragma once

#include <drogon/HttpController.h>

class HelloController : public drogon::HttpController<HelloController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(HelloController::hello, "/api/hello", drogon::Get);
    METHOD_LIST_END

    void hello(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    );
};
