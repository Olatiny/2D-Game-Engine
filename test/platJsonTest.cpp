#include "../server_src/includes.hpp"
#include "../server_src/platform.hpp"

// compile: g++ platJsonTest.cpp ../server_src/{character.cpp,gameObject.cpp,gameManager.cpp,timeline.cpp,platform.cpp} -g -o platJsonTest -lsfml-graphics -lsfml-window -lsfml-system -pthread -lX11
int main(void) {
    std::vector<GameObject*> objs;

    Platform* c = new Platform(Vector2f(5, 2), Color(252, 32, 103, 255), 32, 205, 57, 300, false, true, 75, Platform::counterclockwise, true, true);
    objs.push_back(c);

    Platform* c1 = new Platform(Vector2f(5, 2), Color(13, 253, 12, 255), 44, 123, 2, 14, true, false, 13, Platform::clockwise, true, true);
    objs.push_back(c1);

    // nlohmann::json j1 = c->toJson();
    // nlohmann::json j2 = c1->toJson();

    nlohmann::json jAll;

    // jAll = {j1, j2};
    for (int i = 0; i < objs.size(); i++) {
        jAll.push_back(objs[i]->toJson());
    }

    std::cout << jAll.dump() << std::endl;

    Platform* c1_ret = new Platform(jAll[1]);

    std::cout << c1_ret->speed << std::endl;

    // Platform c2(j1);

    // std::cout << c2.dir << std::endl;
    
    return 0;
}