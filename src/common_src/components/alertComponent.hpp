#pragma once

#include "../gameObject.hpp"
#include "component.hpp"
#include "../eventManager.hpp"
#include "../gameEvent.hpp"

class AlertComponent : public Component {
    private:
        GameObject* obj;
        Timeline* tl;
    public:
        AlertComponent(GameObject* obj, Timeline* tl);

        AlertComponent(GameObject* obj, Timeline* tl, nlohmann::json j);

        void update(std::vector<GameObject*> objects, float dt = 0);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};