#pragma once

#include "../gameObject.hpp"
#include "component.hpp"
#include "../eventManager.hpp"
#include "../gameEvent.hpp"

class RigidBodyComponent : public Component {
    private:
        GameObject* obj;
        Timeline* tl;
        // std::vector<GameObject*>* objs;
        std::mutex* _mutex;
        float g;
    public:
        RigidBodyComponent(GameObject* obj, Timeline* tl, std::mutex* _mutex, float g);

        RigidBodyComponent(nlohmann::json j);

        void update(std::vector<GameObject*> objects, float dt = 0);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};