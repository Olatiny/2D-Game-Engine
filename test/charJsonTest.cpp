#include "../server_src/includes.hpp"
#include "../server_src/character.hpp"

// compile: g++ charJsonTest.cpp ../server_src/{character.cpp,gameObject.cpp,gameManager.cpp,timeline.cpp,platform.cpp} -g -o charJsonTest -lsfml-graphics -lsfml-window -lsfml-system -pthread -lX11 -lzmq
int main(void) {
    Character c(Vector2f(5, 2), Color(252, 32, 103, 255), 32, 205, 57, true, true, true);

    nlohmann::json jData;// = c.toJson();

    jData["id"] = 4;
    jData["player"] = c.toJson();

    std::cout << jData.dump() << std::endl;
    std::cout << jData["id"] << std::endl;
    std::cout << (jData["player"]).dump() << std::endl;

    Character c2(jData["player"]);

    std::cout << c2.getSpeed() << std::endl;
    
    return 0;
}