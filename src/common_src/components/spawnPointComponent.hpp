#pragma once

#include "../gameObject.hpp"
#include "component.hpp"
#include "../eventManager.hpp"
#include "../gameEvent.hpp"

class SpawnPointComponent : public Component {
    private:
        Vector2f loc;
        GameObject* obj;
        GameObject* deathZone;
        Timeline* tl;
    public:
        SpawnPointComponent(GameObject* obj, Timeline* tl, GameObject* deathZone, Vector2f loc);

        void update(std::vector<GameObject*> objects, float dt);

        void onEvent(GameEvent* ev, float dt = 0);

        Vector2f getSpawnPoint();

        nlohmann::json toJson();
};