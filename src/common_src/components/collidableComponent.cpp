#include "collidableComponent.hpp"

CollidableComponent::CollidableComponent(GameObject* obj, Timeline* tl) {
    this->obj = obj;
    this->tl = tl;
    type = Component::COLLIDABLE;
}

CollidableComponent::CollidableComponent(nlohmann::json j) {
    type = static_cast<ComponentType>(j["type"]);
}

void CollidableComponent::update(std::vector<GameObject*> objects, float dt) {
    for (GameObject* g : objects) {
        // if (g->hasComponent(SCREEN_TRANSITION)) {
        //     std::cout << "sending transition event" << std::endl;
        // }
        if (g->hasComponent(ComponentType::COLLIDABLE) && g != obj) {
            if (obj->getGlobalBounds().intersects(g->getGlobalBounds())) {
                // std::cout << "collision id " << obj->getId() << std::endl;
                GameEvent* ev = new GameEvent(GameEvent::COLLISION, GameEvent::HIGH, tl, g, obj);

                nlohmann::json data;

                if (obj->getBottomBox()->getGlobalBounds().intersects(g->getGlobalBounds())) data["bottom"] = true;
                if (obj->getTopBox()->getGlobalBounds().intersects(g->getGlobalBounds())) data["top"] = true;
                if (obj->getLeftBox()->getGlobalBounds().intersects(g->getGlobalBounds())) data["left"] = true;
                if (obj->getRightBox()->getGlobalBounds().intersects(g->getGlobalBounds())) data["right"] = true;

                nlohmann::json j;

                j["directions"] = data;

                ev->data = j;

                EventManager::getInstance()->raise(ev);
            }
        }
    }
}

void CollidableComponent::onEvent(GameEvent* ev, float dt) {

}

nlohmann::json CollidableComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;

    return j;
}
