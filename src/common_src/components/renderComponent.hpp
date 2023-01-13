#pragma once

#include "../gameObject.hpp"
#include "component.hpp"

class RenderComponent : public Component {
    public:
        RenderComponent();

        RenderComponent(nlohmann::json j);

        void update(std::vector<GameObject*> objects, float dt);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};