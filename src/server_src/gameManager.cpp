#include "./gameManager.hpp"

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

void GameManager::startup(v8::Isolate *isolate) {
    this->w = new RenderWindow(VideoMode(400, 400), "Window", Style::Default);

    w->setTitle("Server");

    this->g = 30;

    globalTime = new Timeline(1);
    physicsTime = new Timeline(globalTime, 1);
    renderTime = new Timeline(globalTime, 1);
    last_physics_time = physicsTime->getTime();
    last_render_time = renderTime->getTime();

    context = zmq::context_t(1);

    sock = zmq::socket_t(context, zmq::socket_type::rep);
    sock.bind("tcp://*:1337");
    int t = 1000;
    sock.setsockopt(ZMQ_RCVTIMEO, &t, sizeof(t));

    pub = zmq::socket_t(context, zmq::socket_type::pub);
    pub.bind("tcp://*:6969");
    pub.setsockopt(ZMQ_CONFLATE, 1);

    event_pub = zmq::socket_t(context, zmq::socket_type::pub);
    event_pub.bind("tcp://*:4200");

    f = new sf::Font();
    f->loadFromFile("./resources/Segoe UI.ttf");

    // CollidableComponent* collider = new CollidableComponent();
    RenderComponent* renderer = new RenderComponent();

    // Best practice to isntall all global functions in the context ahead of time.
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    // Bind the global 'print' function to the C++ Print callback.
    global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
    // Bind the global static factory function for creating new GameObject instances
    // global->Set(isolate, "gameobjectfactory", v8::FunctionTemplate::New(isolate, GameObject::ScriptedGameObjectFactory));
    // Bind the global static function for retrieving object handles
    global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
    v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
    v8::Context::Scope default_context_scope(default_context); // enter the context

    sm = new ScriptManager(isolate, default_context);

    GameObject* boundary1 = new GameObject(1200, Vector2f(400, 100), Vector2f(0, -75), Color(100, 175, 20, 255));
    boundary1->addComponent(new CollidableComponent(boundary1, globalTime));
    boundary1->addComponent(renderer);
    objs.push_back(boundary1);

    GameObject* boundary2 = new GameObject(1201, Vector2f(400, 100), Vector2f(0, 375), Color(100, 175, 20, 255));
    boundary2->addComponent(new CollidableComponent(boundary2, globalTime));
    boundary2->addComponent(renderer);
    objs.push_back(boundary2);

    GameObject* boundary3 = new GameObject(1202, Vector2f(50, 350), Vector2f(-25, 25), Color(100, 175, 20, 255));
    boundary3->addComponent(new CollidableComponent(boundary3, globalTime));
    boundary3->addComponent(renderer);
    objs.push_back(boundary3);

    GameObject* boundary4 = new GameObject(1203, Vector2f(50, 350), Vector2f(375, 25), Color(100, 175, 20, 255));
    boundary4->addComponent(new CollidableComponent(boundary4, globalTime));
    boundary4->addComponent(renderer);
    objs.push_back(boundary4);
}

void GameManager::updateObjects() {
    // Timeline physicsTime(globalTime, 1);
    float dt = 0;
    if ((dt = physicsTime->getTime() - last_physics_time) < 1/120.f) return;
    last_physics_time = physicsTime->getTime();
    // physicsTime->reset();
    // std::cout << dt << std::endl;

    // printf("%lf\n", dt);

    // std::cout << dt << std::endl;

    for (int i = 0; i < objs.size(); i++) {
        objs.at(i)->doUpdate(&objs, dt);
    }

    for (int i = 0; i < objs.size(); i++) {
        objs.at(i)->doUpdate(&players, dt);
    }

    for (int i = 0; i < players.size(); i++) {
        Component* c;
        if (c = players[i]->getComponent(Component::TEXT)) {
            c->update(objs, dt);
        }
    }

    // for (int i = 0; i < players.size(); i++) {
    //     players.at(i)->doUpdate(&objs, dt);
    // }

    // for (int i = 0; i < objs.size(); i++) {
    //     objs.at(i)->doCollision(objs, &_mutex);
    // }
}

void GameManager::render() {
    // Timeline renderTime(1);
    // renderTime->reset();
    // std::cout << dt << std::endl;

    // std::unique_lock<std::mutex> lock(_mutex);

    w->clear();

    for (int i = 0; i < objs.size(); i++) {
        if (objs[i]->hasComponent(Component::RENDER)) {
            w->draw(*(objs.at(i)));

            Component* c;
            if (c = objs[i]->getComponent(Component::TEXT)) {
                // std::cout << "drawing text" << std::endl;
                w->draw(*(dynamic_cast<TextComponent*>(c)->text));
            }
            // std::cout << objs[i]->getId() << std::endl;
        }
    }

    // std::cout << players.size() << std::endl;
    for (int i = 0; i < players.size(); i++) {
        // printf("player %d render\n", i);
        if (!(players[i])->isDisconnected()) {
            // printf("player %d not disconnected\n", i);
            if (players[i]->hasComponent(Component::RENDER)) {
                // std::cout << "rendering" << std::endl;
                w->draw(*(players.at(i)));

                Component* c;
                if (c = players[i]->getComponent(Component::TEXT)) {
                    // std::cout << "drawing text" << std::endl;
                    w->draw(*(dynamic_cast<TextComponent*>(c)->text));
                }
            }
        }
        // printf("hi\n");
    }

    float dt = 0;
    if ((dt = renderTime->getTime() - last_render_time) < 1/60.f) return;
    last_render_time = renderTime->getTime();

    w->display();

    // std::cout << "fps: " << 1 / renderTime.getTime() << '\n';
}

void GameManager::shutdown() {
    // Shutdown stuff
    sock.close();
    pub.close();
    event_pub.close();
    
    for (int i = 0; i < subs.size(); i++) {
        subs[i].close();
    }

    context.close();
}