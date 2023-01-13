#include "../src/common_src/includes.hpp"
#include "../src/common_src/character.hpp"
// #include <nlohmann/json.hpp>

using namespace nlohmann;

// compile: g++ jsonTest.cpp ../src/common_src/{character.cpp,gameObject.cpp,timeline.cpp,platform.cpp} -g -o jsonTest -lsfml-graphics -lsfml-window -lsfml-system -pthread -lX11 -lzmq
int main() {
    json j1 = R"(
    {
        "pi": 3.142,
        "happy": true,
        "velocity": [4.4, 52.1]
    }
    )"_json;

    std::cout << j1["pi"] << std::endl;
    float s = j1["velocity"][0];
    std::cout << s + 1 << std::endl;

    Character c(Vector2f(4, 3), Color(5, 5, 5), 3, 6, 13.2);

    json playerJson;

    playerJson["position"] = {c.getPosition().x, c.getPosition().y};
    playerJson["size"] = {c.getSize().x, c.getSize().y};
    playerJson["fillColor"] = {c.getFillColor().r, c.getFillColor().g, c.getFillColor().b, c.getFillColor().a};
    playerJson["borderColor"] = {c.getOutlineColor().r, c.getOutlineColor().g, c.getOutlineColor().b, c.getOutlineColor().a};
    playerJson["outlineThickness"] = c.getOutlineThickness();
    // playerJson["texture"] = c.getTexture().
    playerJson["speed"] = c.getSpeed();
    playerJson["gravityEnabled"] = c.isGravityEnabled();
    playerJson["collisionEnabled"] = c.isCollisionEnabled();
    playerJson["collidable"] = c.isCollidable();
    playerJson["isPlayerControllable"] = c.isPlayerControllable();

    std::cout << playerJson.dump() << std::endl;

    std::string t3 = "{\"hi\": 2}";

    json j3 = json::parse(t3);

    std::cout << j3["hi"] << std::endl;
    std::cout << j3.dump() << std::endl;

    json j4;

    j4["objects"] = {j1, j3};

    std::cout << j4.dump() << std::endl;

    json j5;

    j5.push_back(j3);

    j5["something"] = "worked";

    std::cout << j5[0] << std::endl;
}