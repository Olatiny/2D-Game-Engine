#pragma once

#include "../gameObject.hpp"
#include "component.hpp"

class PlayerComponent : public Component {
    private:
        RenderWindow* window;
        GameObject* obj;
        Timeline* tl;
        std::vector<GameObject*>* objects;
    public:
        Vector2f velocity;
        float speed;
        float gravity;
        bool canJump;

        std::mutex* _mutex;

        PlayerComponent(GameObject* obj, Timeline* tl, Vector2f velocity, float speed, float gravity, bool canJump, RenderWindow* win, std::mutex* _mutex);

        PlayerComponent(nlohmann::json j);

        void update(std::vector<GameObject*> objects, float dt);

        void onEvent(GameEvent* ev, float dt);

        void doMovement(GameObject* obj, float dt = 0);

        void doCollision(GameObject* obj, std::vector<GameObject*> objects);

        nlohmann::json toJson();
};