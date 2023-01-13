#pragma once

#include "../gameObject.hpp"
#include "component.hpp"

class SideBoundaryComponent : public Component {
    private:
        RenderWindow* window;
        int player_id;
        GameObject* obj;
        Timeline* tl;
    public:
        SideBoundaryComponent(GameObject* obj, Timeline* tl, RenderWindow* window, int player_id);

        void update(std::vector<GameObject*> objects, float dt = 0);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};