#pragma once

// #include "../includes.hpp"
#include "../gameObject.hpp"
#include "component.hpp"
#include "../eventManager.hpp"
#include "../gameEvent.hpp"

class ScriptComponent : public Component {
    private:
        GameObject* obj;
        Timeline* tl;
        std::string script_loc;
        ScriptManager *sm;
        v8::Isolate *isolate;
        std::string context_name;
        // int argc;
        // v8::Local<v8::Value> argv[];
        // std::string scriptID;
    public:
        ScriptComponent(GameObject* obj, Timeline* tl, v8::Isolate *isolate, ScriptManager *sm, std::string script_loc, int argc, v8::Local<v8::Value> argv[], std::string context_name = "default");

        ScriptComponent(GameObject* obj, Timeline* tl, v8::Isolate *isolate, ScriptManager *sm, nlohmann::json j, int argc, v8::Local<v8::Value> argv[], std::string context_name = "default");

        void update(std::vector<GameObject*> objects, float dt = 0);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};