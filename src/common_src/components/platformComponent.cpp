#include "platformComponent.hpp"

PlatformComponent::PlatformComponent(GameObject* obj, Timeline* tl, float x_min, float x_max, float y_min, float y_max, float speed, bool isMoving, std::mutex* _mutex, rotation r) {
    this->obj = obj;
    this->tl = tl;
    
    this->type = PLATFORM;
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
    this->speed = speed;
    this->isMoving = isMoving;
    this->rot = r;
    this->_mutex = _mutex;

    dir = startup;
}

PlatformComponent::PlatformComponent(nlohmann::json j) {
    type = static_cast<ComponentType>(j["type"]);
    x_min = j["x_min"];
    x_max = j["x_max"];
    y_min = j["y_min"];
    y_max = j["y_max"];
    speed = j["speed"];
    isMoving = j["isMoving"];
    rot = static_cast<rotation>(j["rot"]);
    dir = static_cast<direction>(j["dir"]);
}

void PlatformComponent::update(std::vector<GameObject*> objects, float dt) {
    if (isMoving) {
        float x = obj->getPosition().x;
        float y = obj->getPosition().y;

        float lspeed = speed * dt;

        switch (this->dir) {
            case right:
                if (x < x_max) {
                    obj->move(Vector2f(lspeed, 0));
                    // getTopBox()->move(Vector2f(lspeed, 0));

                    // for (int i = 0; i < objects.size(); i++) {
                    //     if (objects.at(i)->isPlayerControllable()) {
                    //         GameObject* player = objects.at(i);
                            
                    //         {
                    //             std::unique_lock<std::mutex> lock(*_mutex);
                    //             if (getGlobalBounds().intersects(player->getGlobalBounds())) player->move(Vector2f(lspeed, 0));

                    //             if (friction) {
                    //                 if (getTopBox()->getGlobalBounds().intersects(player->getGlobalBounds()) && !getGlobalBounds().intersects(player->getGlobalBounds())) player->move(Vector2f(lspeed, 0));
                    //             }
                    //         }
                    //     }
                    // }
                } else {
                    this->dir = rot ? down : up;
                }

                break;
            case left:
                if (x > x_min) {
                    obj->move(Vector2f(-lspeed, 0));
                    // getTopBox()->move(Vector2f(-lspeed, 0));
                    // for (int i = 0; i < objects.size(); i++) {
                    //     if (objects.at(i)->isPlayerControllable()) {
                    //         GameObject* player = objects.at(i);

                    //         {
                    //             std::unique_lock<std::mutex> lock(*_mutex);

                    //             if (getGlobalBounds().intersects(player->getGlobalBounds())) player->move(Vector2f(-lspeed, 0));
                                
                    //             if (friction) {
                    //                 if (getTopBox()->getGlobalBounds().intersects(player->getGlobalBounds()) && !getGlobalBounds().intersects(player->getGlobalBounds())) player->move(Vector2f(-lspeed, 0));
                    //             }
                    //         }

                    //     }
                    // }
                } else {
                    this->dir = rot ? up : down;
                }

                break;
            case up:
                if (y > y_min) {
                    obj->move(Vector2f(0, -lspeed));
                    // getTopBox()->move(Vector2f(0, -lspeed));

                    // for (int i = 0; i < objects.size(); i++) {
                    //     if (objects.at(i)->isPlayerControllable()) {
                    //         GameObject* player = objects.at(i);

                    //         {
                    //             std::unique_lock<std::mutex> lock(*_mutex);

                    //             if (getGlobalBounds().intersects(player->getGlobalBounds())) player->move(Vector2f(0, -lspeed));
                    //         }
                    //     }
                    // }   
                } else {
                    this->dir = rot ? right : left;
                }

                break;
            case down:
                if (y < y_max) {
                    obj->move(Vector2f(0, lspeed));
                    // getTopBox()->move(Vector2f(0, lspeed));
                    // for (int i = 0; i < objects.size(); i++) {
                    //     if (objects.at(i)->isPlayerControllable()) {
                    //         GameObject* player = objects.at(i);

                    //         {
                    //             std::unique_lock<std::mutex> lock(*_mutex);

                    //             if (getGlobalBounds().intersects(player->getGlobalBounds())) player->move(Vector2f(0, lspeed));

                    //             if (friction) {
                    //                 if (getTopBox()->getGlobalBounds().intersects(player->getGlobalBounds()) && !getGlobalBounds().intersects(player->getGlobalBounds())) player->move(Vector2f(0, lspeed));
                    //             }
                    //         }
                    //     }
                    // }
                } else {
                    this->dir = rot ? left : right;
                }

                break;
            case startup:
                {
                    std::unique_lock<std::mutex> lock(*_mutex);

                    obj->setPosition(Vector2f(rot ? x_min : x_max, y_min));
                    this->dir = rot ? right : left;
                }
                break;
            default:
                fprintf(stderr, "Something went wrong so we're resetting the moving platform\n");

                {
                    std::unique_lock<std::mutex> lock(*_mutex);

                    obj->setPosition(Vector2f(rot ? x_min : x_max, y_min));
                    this->dir = rot ? right : left;
                }
                break;
        }
    }
}

nlohmann::json PlatformComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;
    j["x_min"] = x_min;
    j["x_max"] = x_max;
    j["y_min"] = y_min;
    j["y_max"] = y_max;
    j["speed"] = speed;
    j["isMoving"] = isMoving;
    j["rot"] = rot;
    j["dir"] = dir;

    return j;
}

void PlatformComponent::onEvent(GameEvent* ev, float dt) {

}
