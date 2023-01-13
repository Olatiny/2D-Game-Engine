#pragma once

#include "includes.hpp"
#include "gameEvent.hpp"
#include "./components/component.hpp"

class EventManager {
    private:
        EventManager() {};
        std::unordered_map<GameEvent::EventType, std::vector<Component*>> registeredComponents;
        std::priority_queue<GameEvent*, std::vector<GameEvent*>, std::greater<GameEvent*>> events;
    public:
        EventManager(EventManager &other) = delete;

        void operator = (const EventManager &) = delete;

        static EventManager* getInstance();

        static std::priority_queue<GameEvent*, std::vector<GameEvent*>, std::greater<GameEvent*>>* getEvents();

        static void registerComponent(Component* obj, GameEvent::EventType type);

        static void unregisterComponent(Component* obj, GameEvent::EventType type);

        static void raise(GameEvent* e);

        static void handleEvents(float dt = 0);

        // static void RaiseV8(const v8::FunctionCallbackInfo<v8::Value>& args);
};