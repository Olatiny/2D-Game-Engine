#pragma once

#include "../gameObject.hpp"
#include "component.hpp"
#include "../eventManager.hpp"
#include "../gameEvent.hpp"

class DeathZoneComponent : public Component {
    private:
        GameObject* obj;
        Timeline* tl;
    public:
        // GameObject* spawn;

        DeathZoneComponent(GameObject* thisObj, Timeline* tl);

        void update(std::vector<GameObject*> objects, float dt = 0);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};