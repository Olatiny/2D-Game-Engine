#include "spawnPointComponent.hpp"

SpawnPointComponent::SpawnPointComponent(GameObject* obj, Timeline* tl, GameObject* deathZone, Vector2f loc) {
    this->loc = loc;
    this->type = SPAWN_POINT;
    this->deathZone = deathZone;
    this->obj = obj;
    this->tl = tl;
}

void SpawnPointComponent::update(std::vector<GameObject*> objects, float dt) {
    // do nothing
}

void SpawnPointComponent::onEvent(GameEvent* ev, float dt) {
    if (ev->type == GameEvent::DEATH) {
        if (ev->withObj == deathZone) {
            // std::cout << "death handled" << std::endl;
            ev->raiseObj->setPosition(loc);
            EventManager::getInstance()->raise(new GameEvent(GameEvent::SPAWN, GameEvent::MEDIUM, tl, ev->raiseObj, obj));
            // std::cout << "spawn raised" << std::endl;
        }
    }
}

Vector2f SpawnPointComponent::getSpawnPoint() {
    return loc;
}

nlohmann::json SpawnPointComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;
    j["location"] = {loc.x, loc.y};
    j["deathZone"] = deathZone->getId();

    return j;
}