#include "scriptComponent.hpp"

ScriptComponent::ScriptComponent(GameObject* obj, Timeline* tl, v8::Isolate *isolate, ScriptManager *sm, std::string script_loc, int argc, v8::Local<v8::Value> argv[], std::string context_name) {
    v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
    v8::HandleScope handle_scope(isolate);

    this->sm = sm;
    this->script_loc = script_loc;
    this->isolate = isolate;
    this->context_name = context_name;
    // this->argc = argc;
    // memcpy(this->argv, argv, sizeof(argv));

    this->sm->addScript(script_loc, script_loc, context_name);
    sm->runOne(script_loc, false, context_name);
    sm->runFunction(script_loc, "start", argc, argv, false, context_name);

    this->obj = obj;
    this->tl = tl;
    
    this->type = SCRIPT;
}

ScriptComponent::ScriptComponent(GameObject* obj, Timeline* tl, v8::Isolate *isolate, ScriptManager *sm, nlohmann::json j, int argc, v8::Local<v8::Value> argv[], std::string context_name) {
    v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
    v8::HandleScope handle_scope(isolate);

    this->sm = sm;
    this->script_loc = j["script_loc"];
    this->isolate = isolate;
    this->context_name = context_name;

    this->sm->addScript(script_loc, script_loc, context_name);
    sm->runOne(script_loc, false, context_name);
    sm->runFunction(script_loc, "start", argc, argv, false, context_name);

    this->obj = obj;
    this->tl = tl;
    
    this->type = SCRIPT;
}

void ScriptComponent::update(std::vector<GameObject*> objects, float dt) {
    v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Value> args[1] = {v8::Number::New(isolate, dt)};

    sm->runFunction(script_loc, "update", 1, args, false, context_name);
}

void ScriptComponent::onEvent(GameEvent* ev, float dt) {
    // std::cout << "got here" << std::endl;

    v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Value> args[2] = {v8::String::NewFromUtf8(isolate, ev->toJson().dump().c_str()),
                                    v8::Number::New(isolate, dt)};


    sm->runFunction(script_loc, "onEvent", 2, args, false, context_name);
}

nlohmann::json ScriptComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["script_loc"] = script_loc;
    j["type"] = type;
    // j[""]

    return j;
}