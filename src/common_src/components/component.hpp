#pragma once

#include "../gameEvent.hpp"
// #include "../gameObject.hpp"
#include "../includes.hpp"
#include "../timeline.hpp"

class GameObject;

class GameEvent;

class Component {
    public:
        // Component() {};

        // Component(nlohmann::json j) {};

        enum ComponentType {COLLIDABLE, TRIGGER, ALERT, RIGID_BODY, RENDER, PLAYER, PLATFORM, SPAWN_POINT, DEATH_ZONE, SCREEN_TRANSITION, TEXT, SCRIPT};

        virtual void update(std::vector<GameObject*> objects, float dt = 0) = 0;

        virtual void onEvent(GameEvent* ev, float dt = 0) = 0;

        virtual nlohmann::json toJson() = 0;

        ComponentType type;
};