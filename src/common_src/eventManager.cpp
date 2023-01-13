#include "eventManager.hpp"

EventManager* EventManager::getInstance() {
    static EventManager eventManager;
    return &eventManager;
}

void EventManager::registerComponent(Component* obj, GameEvent::EventType type) {
    getInstance()->registeredComponents[type].push_back(obj);
}

void EventManager::unregisterComponent(Component* obj, GameEvent::EventType type) {
    int i;
    for (Component* g : getInstance()->registeredComponents[type]) {
        if (g == obj) {
            break;
        }
        i++;
    }
    getInstance()->registeredComponents[type].erase(getInstance()->registeredComponents[type].begin() + i);
}

void EventManager::raise(GameEvent* e) {
    getInstance()->events.push(e);
}

std::priority_queue<GameEvent*, std::vector<GameEvent*>, std::greater<GameEvent*>>* EventManager::getEvents() {
    return &EventManager::getInstance()->events;
};


void EventManager::handleEvents(float dt) {
    int i = 0;

    while (getInstance()->events.size() > 0 && (getInstance()->events.top()->priority == 0 || i < 5)) {
        GameEvent* e = getInstance()->events.top();
        getInstance()->events.pop();

        // if (e->type == GameEvent::COLLISION) {
        //     if (e->raiseObj->getId() == 0) {
        //         std::cout << "collision " << 0 << std::endl;
        //     }
        // }

        // std::cout << getInstance()->events << std::endl;
        // std::cout << (*e) << " handled" << std::endl;

        for (Component* g : getInstance()->registeredComponents[e->type]) {
            // if (!g) continue;
            // std::cout << g->type << std::endl;
            // seg fault before or after?
            // std::cout << "before" << std::endl;
            g->onEvent(e, dt);
            // std::cout << "after" << std::endl;
        }

        i++;
    }
}

// /**
//  * Raises Events
//  */
// void EventManager::RaiseV8(const v8::FunctionCallbackInfo<v8::Value>& args)
// {
// 	v8::Isolate *isolate = args.GetIsolate();
// 	v8::Local<v8::Context> context = isolate->GetCurrentContext();
// 	// std::cout << "raising: " << v8helpers::ToCString(v8::String::Utf8Value(args.GetIsolate(), args[0])) << std::endl;
	
//     getInstance()->raise(new GameEvent(args[0]->Int32Value(), GameEvent::HIGH, ))
    
//     // v8::Local<v8::Value> object = 
// 	// std::string s = 
	
// 	// args.GetReturnValue().Set(object);
// }