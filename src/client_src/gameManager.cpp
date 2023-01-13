#include "gameManager.hpp"
// #include <list>

GameManager* GameManager::getInstance() {
    // if (gameManager) {
    //     return gameManager;
    // } else {
    //     gameManager = new GameManager();
    //     return gameManager;
    // }

    static GameManager gameManager;
    return &gameManager;
}

RenderWindow* GameManager::getWindow() {
    return this->w;
}

std::mutex* GameManager::getMutex() {
    return &(this->_mutex);
}

Timeline* GameManager::getGlobalTime() {
    return this->globalTime;
}

// void GameManager::setTimeMod(float m) {
//     this->time_mod = m;
// }

// float GameManager::getTimeMod() {
//     return this->time_mod;
// }

float GameManager::getGravity() {
    return this->g;
}

void GameManager::setGravity(float g) {
    this->g = g;
}

GameObject* GameManager::getObjectById(int id) {
    for (GameObject* g : players) {
        if (g->getId() == id) {
            return g;
        }
    }
    
    for (GameObject* g : objs) {
        if (g->getId() == id) {
            return g;
        }
    }

    return NULL;
}

/**
 * Factory method for allowing javascript to create instances of native game
 * objects
 *
 * NOTE: Like with the setters above, this static function does have a return
 * type (and object), but the return value is placed in the function callback
 * parameter, not the native c++ return type.
 */
void GameManager::ScriptedGameObjectFactory(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	v8::EscapableHandleScope handle_scope(args.GetIsolate());
	v8::Context::Scope context_scope(context);

	std::string context_name("default");
	if(args.Length() == 1)
	{
		v8::String::Utf8Value str(args.GetIsolate(), args[0]);
		context_name = std::string(v8helpers::ToCString(str));
#if GO_DEBUG
		std::cout << "Created new object in context " << context_name << std::endl;
#endif
	}

    Texture *tx = new Texture();
    tx->loadFromFile("./resources/snake-body.png");

	GameObject *new_object = new GameObject(args[0]->Int32Value(), Vector2f(25, 25), Vector2f(args[1]->Int32Value(), args[2]->Int32Value()), Color(150, 150, 150));
    // new_object->setOrigin(25./2., 25./2.);
    // new_object->setRotation(args[3]->NumberValue());
    new_object->addComponent(new RenderComponent());
    new_object->addComponent(new CollidableComponent(new_object, getInstance()->globalTime));
    getInstance()->objs.push_back(new_object);
	v8::Local<v8::Object> v8_obj = new_object->exposeToV8(isolate, context);
	args.GetReturnValue().Set(handle_scope.Escape(v8_obj));
}

/**
 * Factory method for allowing javascript to create instances of native game
 * objects
 *
 * NOTE: Like with the setters above, this static function does have a return
 * type (and object), but the return value is placed in the function callback
 * parameter, not the native c++ return type.
 */
void GameManager::RotateFromScript(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	v8::EscapableHandleScope handle_scope(args.GetIsolate());
	v8::Context::Scope context_scope(context);

	std::string context_name("default");
	if(args.Length() == 1)
	{
		v8::String::Utf8Value str(args.GetIsolate(), args[0]);
		context_name = std::string(v8helpers::ToCString(str));
#if GO_DEBUG
		std::cout << "Created new object in context " << context_name << std::endl;
#endif
	}

    getInstance()->getObjectById(args[0]->Int32Value())->setRotation(args[1]->NumberValue());
}

/**
 * Factory method for allowing javascript to create instances of native game
 * objects
 *
 * NOTE: Like with the setters above, this static function does have a return
 * type (and object), but the return value is placed in the function callback
 * parameter, not the native c++ return type.
 */
void GameManager::TextureFromScript(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	v8::EscapableHandleScope handle_scope(args.GetIsolate());
	v8::Context::Scope context_scope(context);

	std::string context_name("default");
	if(args.Length() == 1)
	{
		v8::String::Utf8Value str(args.GetIsolate(), args[0]);
		context_name = std::string(v8helpers::ToCString(str));
#if GO_DEBUG
		std::cout << "Created new object in context " << context_name << std::endl;
#endif
	}

    Texture *tx = new Texture();
    tx->loadFromFile(v8helpers::ToCString(v8::String::Utf8Value(isolate, args[1])));

    getInstance()->getObjectById(args[0]->Int32Value())->setTexture(tx);
}

/**
 * Raises Events
 */
void GameManager::RaiseV8(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	// std::cout << "raising: " << v8helpers::ToCString(v8::String::Utf8Value(args.GetIsolate(), args[0])) << std::endl;
	
    switch (args.Length()) {
        case 1:
            EventManager::getInstance()->raise(new GameEvent(static_cast<GameEvent::EventType>(args[0]->Int32Value()),
                                                GameEvent::HIGH,
                                                getInstance()->globalTime,
                                                NULL,
                                                NULL));
            break;
        case 2:
            EventManager::getInstance()->raise(new GameEvent(static_cast<GameEvent::EventType>(args[0]->Int32Value()),
                                                GameEvent::HIGH,
                                                getInstance()->globalTime,
                                                getInstance()->getObjectById(args[1]->Int32Value()),
                                                NULL));
            break;
        case 3:
            EventManager::getInstance()->raise(new GameEvent(static_cast<GameEvent::EventType>(args[0]->Int32Value()),
                                                GameEvent::HIGH,
                                                getInstance()->globalTime,
                                                getInstance()->getObjectById(args[1]->Int32Value()),
                                                getInstance()->getObjectById(args[2]->Int32Value())));
            break;
        case 4: {
            GameEvent* ev = new GameEvent(static_cast<GameEvent::EventType>(args[0]->Int32Value()),
                                                GameEvent::HIGH,
                                                getInstance()->globalTime,
                                                getInstance()->getObjectById(args[1]->Int32Value()),
                                                getInstance()->getObjectById(args[2]->Int32Value()));
            ev->setData(nlohmann::json::parse(v8helpers::ToCString(v8::String::Utf8Value(args[3]->ToString()))));
            EventManager::getInstance()->raise(ev);
            break;
        } default:
            std::cout << "couldn't raise event from script" << std::endl;
            break;
    }
    
    // v8::Local<v8::Value> object = 
	// std::string s = 
	
	// args.GetReturnValue().Set(object);
}

void GameManager::startup(v8::Isolate *isolate) {
    this->w = new RenderWindow(VideoMode(400, 400), "Window", Style::Default);

    this->isolate = isolate;

    // Best practice to isntall all global functions in the context ahead of time.
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    // Bind the global 'print' function to the C++ Print callback.
    global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
    // Bind the global static factory function for creating new GameObject instances
    global->Set(isolate, "gameobjectfactory", v8::FunctionTemplate::New(isolate, ScriptedGameObjectFactory));
    // Bind the global static function for retrieving object handles
    global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
    
    global->Set(isolate, "raise", v8::FunctionTemplate::New(isolate, RaiseV8));

    global->Set(isolate, "rotate", v8::FunctionTemplate::New(isolate, RotateFromScript));

    global->Set(isolate, "settexture", v8::FunctionTemplate::New(isolate, TextureFromScript));
    
    v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
    v8::Context::Scope default_context_scope(default_context); // enter the context

    sm = new ScriptManager(isolate, default_context);

    this->g = 10;

    // this->time_mod = 1;

    globalTime = new Timeline(1);

    physicsTime = new Timeline(globalTime, 1);

    renderTime = new Timeline(globalTime, 1);

    last_physics_time = physicsTime->getTime();

    last_render_time = renderTime->getTime();

    context = zmq::context_t(1);
    sock = zmq::socket_t(context, zmq::socket_type::req);
    sock.connect("tcp://localhost:1337");

    Texture *tx = new Texture();
    tx->loadFromFile("./resources/snake-head.png");

    f = new sf::Font();
    f->loadFromFile("./resources/Segoe UI.ttf");
    GameObject* player = new GameObject(-1, Vector2f(25, 25), Vector2f(150, 150), Color(200, 200, 200));
    player->addComponent(new CollidableComponent(player, globalTime));
    player->addComponent(new RenderComponent());

    nlohmann::json cData = player->toJson();
    sock.send(zmq::buffer(cData.dump()));

    zmq::message_t reply;
    nlohmann::json initial_data;

    sock.recv(reply);

    initial_data = nlohmann::json::parse(reply.to_string());

    id = initial_data["id"];

    player->setId(id);

    player->exposeToV8(isolate, default_context);
    v8::Local<v8::Value> args[1] = {v8::String::NewFromUtf8(isolate, std::to_string(id).c_str())};
    player->addComponent(new ScriptComponent(player, globalTime, isolate, sm, std::string("./src/common_src/scripts/playerScript.js"), 1, args));
    EventManager::getInstance()->registerComponent(player->getComponent(Component::SCRIPT), GameEvent::INPUT);
    EventManager::getInstance()->registerComponent(player->getComponent(Component::SCRIPT), GameEvent::COLLISION);
    EventManager::getInstance()->registerComponent(player->getComponent(Component::SCRIPT), GameEvent::SPAWN);

    // dynamic_cast<TextComponent*>(player->getComponent(Component::TEXT))->text->setString("player " + std::to_string(id));

    w->setTitle("Client: " + std::to_string(id));

    std::stringstream ss;
    ss << "tcp://*:55" << std::setw(2) << std::setfill('0') << id;
    std::string connection = ss.str();
    std::cout << connection << std::endl;
    pub = zmq::socket_t(context, zmq::socket_type::pub);
    pub.bind(connection);
    pub.setsockopt(ZMQ_CONFLATE, 1);

    sub = zmq::socket_t(context, zmq::socket_type::sub);
    const char * val = "";
    sub.setsockopt(ZMQ_CONFLATE, 1);
    sub.connect("tcp://localhost:6969");
    sub.setsockopt(ZMQ_SUBSCRIBE, val, strlen(val));

    event_sub = zmq::socket_t(context, zmq::socket_type::sub);
    // const char * val = "";
    // event_sub.setsockopt(ZMQ_CONFLATE, 0);
    event_sub.connect("tcp://localhost:4200");
    event_sub.setsockopt(ZMQ_SUBSCRIBE, val, strlen(val));

    for (int i = 0; i < initial_data["objs"].size(); i++) {
        objs.push_back(new GameObject(initial_data["objs"][i]));

        nlohmann::json comp_j = initial_data["objs"][i]["components"];

        // std::cout << comp_j << std::endl;

        for (int j = 0; j < comp_j.size(); j++) {
            // std::cout << comp_j[i] << std::endl;
            switch (static_cast<Component::ComponentType>(comp_j[j]["type"])) {
                case Component::COLLIDABLE:
                    objs[i]->addComponent(new CollidableComponent(objs[i], globalTime));
                    break;
                case Component::TRIGGER:
                    objs[i]->addComponent(new TriggerComponent(objs[i], globalTime));
                    break;
                case Component::RIGID_BODY:
                    objs[i]->addComponent(new RigidBodyComponent(objs[i], globalTime, &_mutex, g));
                    break;
                case Component::RENDER:
                    objs[i]->addComponent(new RenderComponent());
                    break;
                case Component::TEXT:
                    objs[i]->addComponent(new TextComponent(objs[i], globalTime, f, comp_j[j]));
                    break;
                case Component::PLAYER:
                    // components.push_back(new PlayerComponent())
                    break;
                case Component::PLATFORM:
                    // components.push_back(new PlatformComponent(comp_j[i]));
                    break;
                case Component::SPAWN_POINT:
                    for(GameObject* o : objs) {
                        if (o->getId() == comp_j[j]["deathZone"]) {
                            objs[i]->addComponent(new SpawnPointComponent(objs[i], globalTime, o, Vector2f(comp_j[j]["location"][0], comp_j[j]["location"][1])));
                        }
                    }
                    break;
                case Component::DEATH_ZONE:
                    objs[i]->addComponent(new DeathZoneComponent(objs[i], globalTime));
                    break;
                case Component::SCREEN_TRANSITION:
                    objs[i]->addComponent(new SideBoundaryComponent(objs[i], globalTime, w, id));
                    break;
            }
        }

        if (objs[i]->hasComponent(Component::COLLIDABLE)) {
            EventManager::getInstance()->registerComponent(objs[i]->getComponent(Component::COLLIDABLE), GameEvent::COLLISION);
        }
        
        if (objs[i]->hasComponent(Component::DEATH_ZONE)) {
            EventManager::getInstance()->registerComponent(objs[i]->getComponent(Component::DEATH_ZONE), GameEvent::COLLISION);
        }
        
        if (objs[i]->hasComponent(Component::SCREEN_TRANSITION)) {
            EventManager::getInstance()->registerComponent(objs[i]->getComponent(Component::SCREEN_TRANSITION), GameEvent::COLLISION);
        }

        if (objs[i]->hasComponent(Component::SPAWN_POINT)) {
            EventManager::getInstance()->registerComponent(objs[i]->getComponent(Component::SPAWN_POINT), GameEvent::DEATH);
            // std::cout << "added spawn"
        }
    }

    v8::Local<v8::Context> appleContext = v8::Context::New(isolate, NULL, global);
    sm->addContext(isolate, appleContext, "appleContext");

    Texture* t = new Texture();
    t->loadFromFile("./resources/apple.png");

    GameObject* apple = new GameObject(1004, Vector2f(25, 25), Vector2f(250, 75), t);
    apple->setTextureRect(IntRect(0, 0, 700, 700));
    apple->exposeToV8(isolate, appleContext);
    apple->addComponent(new TriggerComponent(apple, globalTime));
    apple->addComponent(new RenderComponent());
    v8::Local<v8::Value> appArgs[2] = {v8::String::NewFromUtf8(isolate, std::to_string(apple->getId()).c_str()),
                                       v8::String::NewFromUtf8(isolate, std::to_string(id).c_str())};
    apple->addComponent(new ScriptComponent(apple, globalTime, isolate, sm, std::string("./src/common_src/scripts/appleScript.js"), 2, appArgs, "appleContext"));
    EventManager::getInstance()->registerComponent(apple->getComponent(Component::SCRIPT), GameEvent::COLLISION);
    EventManager::getInstance()->registerComponent(apple->getComponent(Component::SCRIPT), GameEvent::DEATH);
    objs.push_back(apple);

    v8::Local<v8::Context> alertContext = v8::Context::New(isolate, NULL, global);
    sm->addContext(isolate, alertContext, "alertContext");

    GameObject* alertObj = new GameObject(1005, Vector2f(100, 100), Vector2f(75, 75), Color::Transparent);
    alertObj->exposeToV8(isolate, alertContext);
    alertObj->addComponent(new RenderComponent());
    alertObj->addComponent(new TextComponent(alertObj, globalTime, "", f, "./resources/Segoe UI.ttf", 25, Color::White, static_cast<sf::Text::Style>(Text::Bold), alertObj->getPosition()));
    v8::Local<v8::Value> alertArgs[2] = {v8::String::NewFromUtf8(isolate, std::to_string(alertObj->getId()).c_str()),
                                       v8::String::NewFromUtf8(isolate, std::to_string(id).c_str())};
    alertObj->addComponent(new ScriptComponent(alertObj, globalTime, isolate, sm, std::string("./src/common_src/scripts/death.js"), 1, alertArgs, "alertContext"));
    EventManager::getInstance()->registerComponent(alertObj->getComponent(Component::TEXT), GameEvent::TEXT);
    EventManager::getInstance()->registerComponent(alertObj->getComponent(Component::SCRIPT), GameEvent::DEATH);
    objs.push_back(alertObj);

    v8::Local<v8::Context> scoreContext = v8::Context::New(isolate, NULL, global);
    sm->addContext(isolate, scoreContext, "scoreContext");

    GameObject* scoreObj = new GameObject(1006, Vector2f(100, 100), Vector2f(175, -5), Color::Transparent);
    scoreObj->exposeToV8(isolate, scoreContext);
    scoreObj->addComponent(new RenderComponent());
    scoreObj->addComponent(new TextComponent(scoreObj, globalTime, "score: 0", f, "./resources/Segoe UI.ttf", 25, Color::Black, static_cast<sf::Text::Style>(Text::Bold), scoreObj->getPosition()));
    v8::Local<v8::Value> scoreArgs[2] = {v8::String::NewFromUtf8(isolate, std::to_string(scoreObj->getId()).c_str()),
                                       v8::String::NewFromUtf8(isolate, std::to_string(id).c_str())};
    scoreObj->addComponent(new ScriptComponent(scoreObj, globalTime, isolate, sm, std::string("./src/common_src/scripts/scoreTracker.js"), 1, scoreArgs, "scoreContext"));
    EventManager::getInstance()->registerComponent(scoreObj->getComponent(Component::TEXT), GameEvent::TEXT);
    EventManager::getInstance()->registerComponent(scoreObj->getComponent(Component::SCRIPT), GameEvent::JOIN);
    objs.push_back(scoreObj);

    EventManager::getInstance()->raise(new GameEvent(GameEvent::JOIN, GameEvent::MEDIUM, globalTime, player, objs[objs.size() - 1]));

    for (int i = 0; i < initial_data["players"].size(); i++) {
        if (i == id) {
            players.push_back(player);
        } else {
            players.push_back(new GameObject(initial_data["players"][i]));

            nlohmann::json comp_j = initial_data["players"][i]["components"];

            // std::cout << "player " << i << " " << comp_j << std::endl;

            for (int j = 0; j < comp_j.size(); j++) {
                // std::cout << "player component " << comp_j[i] << std::endl;
                switch (static_cast<Component::ComponentType>(comp_j[j]["type"])) {
                    case Component::COLLIDABLE:
                        players[i]->addComponent(new CollidableComponent(players[i], globalTime));
                        break;
                    case Component::TRIGGER:
                        players[i]->addComponent(new TriggerComponent(players[i], globalTime));
                        break;
                    case Component::TEXT:
                        comp_j[j]["string"] = "player " + std::to_string(i);
                        players[i]->addComponent(new TextComponent(players[i], globalTime, f, comp_j[j]));
                        break;
                    case Component::RIGID_BODY:
                        // players[i]->addComponent(new RigidBodyComponent(players[i], globalTime, &_mutex, g));
                        break;
                    case Component::RENDER:
                        players[i]->addComponent(new RenderComponent());
                        break;
                    case Component::PLAYER:
                        // components.push_back(new PlayerComponent())
                        break;
                    case Component::PLATFORM:
                        // components.push_back(new PlatformComponent(comp_j[i]));
                        break;
                    case Component::SPAWN_POINT:
                        for(GameObject* o : objs) {
                            if (o->getId() == comp_j[j]["deathZone"]) {
                                players[i]->addComponent(new SpawnPointComponent(players[i], globalTime, o, Vector2f(comp_j[j]["location"][0], comp_j[j]["location"][1])));
                            }
                        }
                        break;
                    case Component::DEATH_ZONE:
                        players[i]->addComponent(new DeathZoneComponent(players[i], globalTime));
                        break;
                    case Component::SCREEN_TRANSITION:
                        players[i]->addComponent(new SideBoundaryComponent(players[i], globalTime, w, id));
                        break;
                }
            }

            if (players[i]->hasComponent(Component::COLLIDABLE)) {
                EventManager::getInstance()->registerComponent(players[i]->getComponent(Component::COLLIDABLE), GameEvent::COLLISION);
            }
            
            if (players[i]->hasComponent(Component::DEATH_ZONE)) {
                EventManager::getInstance()->registerComponent(players[i]->getComponent(Component::DEATH_ZONE), GameEvent::COLLISION);
            }
            
            if (players[i]->hasComponent(Component::SCREEN_TRANSITION)) {
                EventManager::getInstance()->registerComponent(players[i]->getComponent(Component::SCREEN_TRANSITION), GameEvent::COLLISION);
            }

            if (players[i]->hasComponent(Component::SPAWN_POINT)) {
                EventManager::getInstance()->registerComponent(players[i]->getComponent(Component::SPAWN_POINT), GameEvent::DEATH);
            }
        }
    }
}

void GameManager::handleEvents() {
    EventManager::getInstance()->handleEvents(physics_dt);
}

void GameManager::updateObjects() {
    // Timeline physicsTime(globalTime, 1);
    // float dt = 0;
    if ((physics_dt = physicsTime->getTime() - last_physics_time) < 1/120.f) return;
    // physics_dt = physicsTime->getTime() - last_physics_time;
    last_physics_time = physicsTime->getTime();
    // physicsTime->reset();
    // std::cout << dt << std::endl;

    // printf("%lf\n", dt);
    
    std::vector<GameObject*> temp(objs);
    std::vector<GameObject*> temp2;

    for (GameObject* g : players) {
        if (!(g)->isDisconnected()) {
            temp2.push_back(g);
        }
    }
    
    temp.insert(temp.end(), temp2.begin(), temp2.end());

    // std::cout << temp2.size() << " " << players.size() << std::endl;

    // players.at(id)->doCollision(temp, &_mutex);

    // std::cout << dt << std::endl;
    players.at(id)->doUpdate(&temp, physics_dt);

    for (GameObject* g : temp) {
        if (g->hasComponent(Component::SCREEN_TRANSITION) || g->hasComponent(Component::DEATH_ZONE) || g->hasComponent(Component::TEXT) || g->hasComponent(Component::TRIGGER)) {
            g->doUpdate(&temp, physics_dt);
        }
    }

    // EventManager::getInstance()->handleEvents(physics_dt);

    // players.at(id)->doCollision(players, &_mutex);

    // for (int i = 0; i < objs.size(); i++) {
    //     objs.at(i)->updateObject(objs, dt = physicsTime.getTime());
    // }

    // for (int i = 0; i < objs.size(); i++) {
    //     objs.at(i)->doCollision(objs);
    // }
}

void GameManager::getServerUpdates() {
    zmq::message_t info;
    sub.recv(info, zmq::recv_flags::dontwait);

    if (info.size()) {
        nlohmann::json initial_data = nlohmann::json::parse(info.to_string());

        // std::cout << initial_data[""] << std::endl;

        for (int i = 0; i < initial_data["objs"].size(); i++) {
            objs.at(i)->doUpdate(initial_data["objs"][i]);
        }

        if (initial_data["players"].size() > players.size()) {
            int i;
            for (i = 0; i < players.size(); i++) {
                if (i == id) continue;
                // if (initial_data["players"][i]["disconnected"] == true) {
                    
                // }
                (players.at(i))->doUpdate(initial_data["players"][i]);
            }

            players.push_back(new GameObject(initial_data["players"][i]));

            nlohmann::json comp_j = initial_data["players"][i]["components"];

            for (int j = 0; j < comp_j.size(); j++) {
                // std::cout << comp_j[i] << std::endl;
                switch (static_cast<Component::ComponentType>(comp_j[j]["type"])) {
                    case Component::COLLIDABLE:
                        players[i]->addComponent(new CollidableComponent(players[i], globalTime));
                        break;
                    case Component::TRIGGER:
                        players[i]->addComponent(new TriggerComponent(players[i], globalTime));
                        break;
                    case Component::RIGID_BODY:
                        players[i]->addComponent(new RigidBodyComponent(players[i], globalTime, &_mutex, g));
                        break;
                    case Component::RENDER:
                        players[i]->addComponent(new RenderComponent());
                        break;
                    case Component::TEXT:
                        comp_j[j]["string"] = "player " + std::to_string(i);
                        players[i]->addComponent(new TextComponent(players[i], globalTime, f, comp_j[j]));
                        break;
                    case Component::PLAYER:
                        // components.push_back(new PlayerComponent())
                        break;
                    case Component::PLATFORM:
                        // components.push_back(new PlatformComponent(comp_j[i]));
                        break;
                    case Component::SPAWN_POINT:
                        for(GameObject* o : objs) {
                            if (o->getId() == comp_j[j]["deathZone"]) {
                                players[i]->addComponent(new SpawnPointComponent(players[i], globalTime, o, Vector2f(comp_j[j]["location"][0], comp_j[j]["location"][1])));
                            }
                        }
                        break;
                    case Component::DEATH_ZONE:
                        players[i]->addComponent(new DeathZoneComponent(players[i], globalTime));
                        break;
                    case Component::SCREEN_TRANSITION:
                        players[i]->addComponent(new SideBoundaryComponent(players[i], globalTime, w, id));
                        break;
                }
            }

            if (players[i]->hasComponent(Component::COLLIDABLE)) {
                EventManager::getInstance()->registerComponent(players[i]->getComponent(Component::COLLIDABLE), GameEvent::COLLISION);
            }
            
            if (players[i]->hasComponent(Component::DEATH_ZONE)) {
                EventManager::getInstance()->registerComponent(players[i]->getComponent(Component::DEATH_ZONE), GameEvent::COLLISION);
            }
            
            if (players[i]->hasComponent(Component::SCREEN_TRANSITION)) {
                EventManager::getInstance()->registerComponent(players[i]->getComponent(Component::SCREEN_TRANSITION), GameEvent::COLLISION);
            }

            if (players[i]->hasComponent(Component::SPAWN_POINT)) {
                EventManager::getInstance()->registerComponent(players[i]->getComponent(Component::SPAWN_POINT), GameEvent::DEATH);
            }

            // EventManager::getInstance()->raise(new GameEvent(GameEvent::JOIN, GameEvent::MEDIUM, globalTime, players[i], objs[objs.size() - 1]));
        } else {
            for (int i = 0; i < players.size(); i++) {
                if (i == id) continue;
                // if (initial_data["players"][i]["disconnected"] == true) {
                    
                // }
                // std::cout << i << " " << initial_data["players"][i] << std::endl;
                (players.at(i))->doUpdate(initial_data["players"][i]);
                // std::cout << "did it" << std::endl;
            }
        }
        // std::cout << initial_data["players"] << std::endl;
    }
}

void GameManager::getServerEvents() {
    zmq::message_t info;
    event_sub.recv(info, zmq::recv_flags::dontwait);

    if (info.size() <= 0) return;

    nlohmann::json initial_data = nlohmann::json::parse(info.to_string());

    if (!initial_data["events"].is_null()) {
            for (int i = 0; i < initial_data["events"].size(); i++) {
                nlohmann::json j = initial_data["events"][i];
                int raiseID = j["raiseObjID"];
                int withID = j["withObjID"];

                GameObject* raiseObj;
                GameObject* withObj;

                if (raiseID <= 99) {
                    if (raiseID == id) std::cout << "it me!" << std::endl;
                    raiseObj = players[raiseID];
                } else {
                    for (GameObject* g : objs) {
                        if (g->getId() == raiseID) {
                            raiseObj = g;
                            break;
                        }
                    }
                }

                if (withID >= 0) {
                    if (withID <= 99) {
                        withObj = players[withID];
                    } else {
                        for (GameObject* g : objs) {
                            if (g->getId() == withID) {
                                withObj = g;
                                break;
                            }
                        }
                    }
                }

                EventManager::raise(new GameEvent(j, raiseObj, withObj));
            }
        }
}

void GameManager::sendToServer() {
    nlohmann::json sData;

    sData["id"] = id;
    sData["player"] = players[id]->toJson();

    // std::cout << sData << std::endl;

    pub.send(zmq::buffer(sData.dump()));
}

void GameManager::render() {
    // Timeline renderTime(1);
    // renderTime->reset();
    // std::cout << dt << std::endl;

    // if (EventManager::getInstance()->getEvents()->top()->priority == 0) return;

    // float dt = 0;
    // if ((render_dt = renderTime->getTime() - last_render_time) < 1/60.f) return;
    render_dt = renderTime->getTime() - last_render_time;
    last_render_time = renderTime->getTime();


    // std::unique_lock<std::mutex> lock(_mutex);

    w->clear();

    for (int i = 0; i < objs.size(); i++) {
        if (objs[i]->hasComponent(Component::RENDER) && !objs[i]->hasComponent(Component::TEXT)) {
            w->draw(*(objs.at(i)));
        }
    }

    for (int i = 0; i < objs.size(); i++) {
        if (objs[i]->hasComponent(Component::RENDER) && objs[i]->hasComponent(Component::TEXT)) {
            w->draw(*(objs.at(i)));

            Component* c;
            if (c = objs[i]->getComponent(Component::TEXT)) {
                w->draw(*(dynamic_cast<TextComponent*>(c)->text));
            }
        }
    }

    for (int i = 0; i < players.size(); i++) {
        if ((players[i])->isDisconnected()) continue;
        if (players[i]->hasComponent(Component::RENDER)) {
            w->draw(*(players.at(i)));

            Component* c;
            if (c = players[i]->getComponent(Component::TEXT)) {
                // std::cout << "drawing text" << std::endl;
                w->draw(*(dynamic_cast<TextComponent*>(c)->text));
            }
        }
    }

    w->display();

    // std::cout << "fps: " << 1 / renderTime.getTime() << '\n';
}

void GameManager::shutdown() {
    // Do shutdown stuff
    nlohmann::json disconnect;
    disconnect["id"] = id;
    disconnect["disconnecting"] = true;
    sock.send(zmq::buffer(disconnect.dump()), zmq::send_flags::dontwait);
    sock.close();
    pub.close();
    sub.close();
    event_sub.close();
    context.close();
}