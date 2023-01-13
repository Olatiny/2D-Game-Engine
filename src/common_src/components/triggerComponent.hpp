#pragma once

#include "../gameObject.hpp"
#include "component.hpp"
#include "../eventManager.hpp"
#include "../gameEvent.hpp"

class TriggerComponent : public Component {
    private:
        GameObject* obj;
        Timeline* tl;
    public:
        TriggerComponent(GameObject* obj, Timeline* tl);

        TriggerComponent(nlohmann::json j);

        void update(std::vector<GameObject*> objects, float dt = 0);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};