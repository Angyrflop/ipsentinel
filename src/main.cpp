// #include <crow.h>
#include <iostream>
// #include <nlohmann/json.hpp>
#include <arpa/inet.h>
#include <vector>
#include <algorithm>
#include <string>

// using json = nlohmann::json;

enum IPFlag :   uint32_t
{
    FLAG_NONE           = 0,
    FLAG_BLACKLISTED    = 1 << 0,
    FLAG_WHITELISTED    = 1 << 1,
    FLAG_HIGH_TRAFIC    = 1 << 2,
    FLAG_RATE_LIMITED   = 1 << 3,
    FLAG_SCANNER        = 1 << 4,
    FLAG_MALICIOUS      = 1 << 5,
    FLAG_INTERNAL       = 1 << 6
};

struct IPEntry
{
    uint32_t addr;
    uint32_t flags;
    int callCount;
};


int main()
{

    uint32_t lolcat = FLAG_WHITELISTED | FLAG_INTERNAL;

    std::cout << "Maybe "<< lolcat;



    // crow::SimpleApp app;

    // const float temperature{25.6f};
    // const float humidity{55.9f};
    // const bool isLightOn{true};

    // json j;
    // j["temperature"] = temperature;
    // j["humidity"] = humidity;
    // j["light"] = isLightOn;

    // CROW_ROUTE(app, "/")([]()
    // {
    //     auto page = crow::mustache::load_text("index.html");
    //     return page;
    // });

    // CROW_ROUTE(app, "/style.css")([](const crow::request&, crow::response& res)
    // {
    //     res.set_static_file_info("style.css");
    //     res.end();
    // });

    // CROW_ROUTE(app, "/script.js")([](const crow::request&, crow::response& res)
    // {
    //     res.set_static_file_info("script.js");
    //     res.end();
    // });

    // CROW_ROUTE(app, "/getReading/kitchen").methods(crow::HTTPMethod::GET)([j](){
    //     return j.dump(0);
    // });

    // CROW_ROUTE(app, "/getReading/bathroom").methods(crow::HTTPMethod::GET)([j](){
    //     return j.dump(0);
    // });

    // CROW_ROUTE(app, "/getReading/bedroom1").methods(crow::HTTPMethod::GET)([j](){
    //     return j.dump(0);
    // });

    // CROW_ROUTE(app, "/getReading/bedroom2").methods(crow::HTTPMethod::GET)([j](){
    //     return j.dump(0);
    // });

    // app.port(2048).multithreaded().run();
    return 0;
}