#include "deathZoneComponent.hpp"
#include "spawnPointComponent.hpp"

DeathZoneComponent::DeathZoneComponent(GameObject* obj, Timeline* tl) {
    type = ComponentType::DEATH_ZONE;
    this->obj = obj;
    // this->spawn = spawn;
    this->tl = tl;
}

void DeathZoneComponent::update(std::vector<GameObject*> objects, float dt) {
    // for (GameObject* o : objects) {
    //     if (o->hasComponent(PLAYER)) {
    //         if (o->getGlobalBounds().intersects(obj->getGlobalBounds())) {
    //             o->setPosition(dynamic_cast<SpawnPointComponent*>(spawn->getComponent(SPAWN_POINT))->getSpawnPoint());
    //         }
    //     }
    // }
}

void DeathZoneComponent::onEvent(GameEvent* ev, float dt) {
    if (ev->type == GameEvent::COLLISION) {
        if (ev->withObj == obj) {
            EventManager::getInstance()->raise(new GameEvent(GameEvent::DEATH, GameEvent::MEDIUM, tl, ev->raiseObj, obj));
            // std::cout << "death raised" << std::endl;
        }
    }
}

nlohmann::json DeathZoneComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;
    // j["spawn"] = spawn->getId();

    return j;
}