#pragma once

#include "../gameObject.hpp"

class PlatformComponent : public Component {
    private:
        GameObject* obj;
        Timeline* tl;

        float x_min;
        float x_max;
        float y_min;
        float y_max;
        float speed;
        bool isMoving;

        /** Enumeration for current direction of platform */
        enum direction {right, down, left, up, startup};
        /** Enumeration for rotation direction*/
        enum rotation {counterclockwise, clockwise};

        direction dir;
        rotation rot;

        std::mutex* _mutex;
    public:
        PlatformComponent(GameObject* obj, Timeline* tl, float x_min, float x_max, float y_min, float y_max, float speed, bool isMoving, std::mutex* _mutex, rotation r = rotation::clockwise);

        PlatformComponent(nlohmann::json j);

        void update(std::vector<GameObject*> objects, float dt);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};