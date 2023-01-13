#include "triggerComponent.hpp"

TriggerComponent::TriggerComponent(GameObject* obj, Timeline* tl) {
    this->obj = obj;
    this->tl = tl;
    type = Component::TRIGGER;
}

TriggerComponent::TriggerComponent(nlohmann::json j) {
    type = static_cast<ComponentType>(j["type"]);
}

void TriggerComponent::update(std::vector<GameObject*> objects, float dt) {
    for (GameObject* g : objects) {
        if (g->hasComponent(ComponentType::COLLIDABLE) && g != obj) {
            if (obj->getGlobalBounds().intersects(g->getGlobalBounds())) {
                // if (obj->hasComponent(SCREEN_TRANSITION)) {
                //     std::cout << "sending transition event" << std::endl;
                // }
                EventManager::getInstance()->raise(new GameEvent(GameEvent::COLLISION, GameEvent::HIGH, tl, g, obj));
            }
        }
    }
}

void TriggerComponent::onEvent(GameEvent* ev, float dt) {

}

nlohmann::json TriggerComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;

    return j;
}
