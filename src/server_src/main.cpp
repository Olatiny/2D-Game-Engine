#include "../common_src/includes.hpp"
#include "gameManager.hpp"

class GameThread {
    public:
        GameManager* manager;
        v8::Isolate *isolate;
        // std::condition_variable* _condition_variable;

        GameThread(GameManager* manager, v8::Isolate *isolate/*, std::condition_variable* _condition_variable*/) {
            this->manager = manager;
            this->isolate = isolate;
        }

        virtual void run() = 0;
};

class PhysicsThread : GameThread {
    public:
        using GameThread::GameThread;

        void run() {
            // v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
            // v8::HandleScope handle_scope(isolate);

            while (this->manager->getWindow()->isOpen()) {
                // printf("physics\n");
                // this->manager->updateObjects();
            }
        }
};

class RenderThread : GameThread {
    bool canClose = false;

    public:
        bool isActive = true;
        // using GameThread::GameThread;
        using GameThread::GameThread;

        void run() {
            this->manager->getWindow()->setActive(true);

            while (this->manager->getWindow()->isOpen()) {
                // printf("render\n");
                // if (manager->getWindow()->hasFocus()) {
                    // manager->getWindow()->setActive(true);
                    this->manager->render();
                // } else {
                    // manager->getWindow()->setActive(false);
                // }

                if (canClose){
                    this->manager->getWindow()->setActive(false);
                    isActive = false;
                    break;
                }

                // std::cout << manager->getWindow()->hasFocus() << std::endl;
            }
        }

        void closeWindow() {
            canClose = true;
        }
};

// class NetworkThread : GameThread {
//     private:
//         std::mutex m;
//         bool init = false;
//         std::string s;
//     public:
//         std::condition_variable cv;

//         using GameThread::GameThread;

//         void setInit(bool init, std::string s) {
//             std::unique_lock<std::mutex> lock(m);
//             this->init = init;
//             this->s = s;
//             cv.notify_all();
//         }

//         void run() {
//             std::unique_lock<std::mutex> lock(m);
//             while (!init) {
//                 cv.wait(lock);
//             }

//             // printf("physics\n");
//             // this->manager->updateObjects();
//             // zmq::message_t req;
//             // manager->sock.recv(req);

//             if (s.find("initial") != std::string::npos) {
//             }
//         }
// };

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper(PhysicsThread *fe)
{
    fe->run();
}

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper2(RenderThread *fe)
{
    fe->run();
}

// void network_wrapper(NetworkThread *fe) {
//     fe->run();
// }

int main(int argc, char *argv[]) {
    XInitThreads();

    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.release());
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);

    GameManager* manager = GameManager::getInstance();
    // std::condition_variable c;

    enum scaleMode {proportional, constant, follow};
    scaleMode mode = proportional;

    {
        v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
        v8::HandleScope handle_scope(isolate);

        manager->startup(isolate);

        // manager->sub.connect("tcp://localhost:4203");
        // const char *str = "";
        // manager->sub.setsockopt(ZMQ_SUBSCRIBE, str, strlen(str));

        RenderWindow* win = manager->getWindow();
        const View originalView = win->getView();
        View view = win->getView();

        win->setActive(false);

        PhysicsThread p(manager, isolate);
        RenderThread c(manager, isolate);
        // NetworkThread n(manager);

        std::thread movement(run_wrapper, &p);
        std::thread render(run_wrapper2, &c);
        // std::thread net(network_wrapper, &n);

        while (win->isOpen()) {
            Event event;

            while (win->pollEvent(event)) {
                if (event.type == Event::Closed || (event.type == event.KeyPressed && event.key.code == Keyboard::Escape)){
                    // c.closeWindow();
                    win->close();
                }

                if (event.type == Event::KeyPressed) {
                    if (Keyboard::isKeyPressed(Keyboard::M)) {
                        if (mode < 2) mode = (scaleMode)(((int) mode) + 1);
                        else mode = proportional;

                        if (mode == proportional) win->setView(originalView);

                        printf("%s\n", mode == 0 ? "proportional" : (mode == 1 ? "constant" : "follow"));
                    }
                } 

                if (event.type == Event::Resized) {
                    switch (mode) {
                        case proportional:
                            // printf("%d %lf\n", event.size.height, view.getSize().y);
                            // if (event.size.height > view.getSize().y) {
                            //     window.setSize(Vector2u(view.getSize().x / view.getSize().y * event.size.height, event.size.height));
                            // } else if (event.size.width > window.getSize().x) {
                            //     window.setSize(Vector2u(event.size.width, view.getSize().y / view.getSize().x * event.size.width));
                            // } else {

                            // }
                            break;
                        case constant:
                            view.setSize(Vector2f(win->getSize()));
                            view.setCenter(win->getSize().x/2, win->getSize().y/2);
                            win->setView(view);
                            break;
                        default:
                            break;
                    }
                }

                if (event.type == Event::KeyPressed) {
                    switch (event.key.code) {
                        case Keyboard::Right: {
                            View v = manager->getWindow()->getView();
                            v.move(Vector2f(1280, 0));
                            manager->getWindow()->setView(v);
                            break;
                        } case Keyboard::Left: {
                            View v = manager->getWindow()->getView();
                            v.move(Vector2f(-1280, 0));
                            manager->getWindow()->setView(v);
                            break;
                        } case Keyboard::Up: {
                            View v = manager->getWindow()->getView();
                            v.move(Vector2f(0, -720));
                            manager->getWindow()->setView(v);
                            break;
                        } case Keyboard::Down: {
                            View v = manager->getWindow()->getView();
                            v.move(Vector2f(0, 720));
                            manager->getWindow()->setView(v);
                            break;
                        }
                    }
                }

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::P) {
                    if (manager->getGlobalTime()->isPaused()) {
                        manager->getGlobalTime()->unpause();
                        printf("unpaused\n");
                    } else {
                        manager->getGlobalTime()->pause();
                        printf("paused\n");
                    }
                }

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::T) {
                    const float tic = manager->getGlobalTime()->getTic();

                    if (tic == 0.5) {
                        manager->getGlobalTime()->changeTic(1);
                    } else if (tic == 1) {
                        manager->getGlobalTime()->changeTic(2);
                    } else if (tic == 2) {
                        manager->getGlobalTime()->changeTic(0.5);
                    } else {
                        manager->getGlobalTime()->changeTic(1);
                    }

                    printf("%lf\n", manager->getGlobalTime()->getTic());
                }
            }

            manager->updateObjects();

            zmq::message_t m;
            manager->sock.recv(m, zmq::recv_flags::dontwait);

            nlohmann::json events;

            bool newChar = false;

            // std::cout << manager->players.size() << std::endl;

            if (m.size() && (m.to_string().find("disconnecting") == std::string::npos)) {
                newChar = true;
                // printf("got update\n");

                // parse message as JSON
                nlohmann::json j = nlohmann::json::parse(m.to_string());

                // std::cout << j << std::endl;

                // add new player
                // printf("adding new player\n");
                // Logic for getting json information for player object
                GameObject* p = new GameObject(j);

                nlohmann::json comp_j = j["components"];

                for (int i = 0; i < comp_j.size(); i++) {
                    switch (static_cast<Component::ComponentType>(comp_j[i]["type"])) {
                        case Component::COLLIDABLE:
                            p->addComponent(new CollidableComponent(p, manager->getGlobalTime()));
                            break;
                        case Component::RENDER:
                            p->addComponent(new RenderComponent());
                            break;
                        case Component::TEXT:
                            comp_j[i]["string"] = "player " + std::to_string(manager->players.size());
                            p->addComponent(new TextComponent(p, manager->getGlobalTime(), manager->f, comp_j[i]));
                            break;
                        case Component::PLAYER:
                            // components.push_back(new PlayerComponent())
                            break;
                        case Component::PLATFORM:
                            // components.push_back(new PlatformComponent(comp_j[i]));
                            break;
                        case Component::SPAWN_POINT:
                            // components.push_back(new SpawnComponent(comp_j[i]));
                            break;
                        case Component::DEATH_ZONE:
                            // components.push_back(new DeathComponent(comp_j[i]));
                            break;
                        case Component::SCREEN_TRANSITION:
                            // p->addComponent(new SideBoundaryComponent(w, id));
                            break;
                    }   
                }

                // std::cout << "render comp at: " << p->hasComponent(Component::RENDER) << std::endl;

                // Put player object in players array
                manager->players.push_back(p);

                p->setId(manager->players.size() - 1);

                nlohmann::json objs;
                nlohmann::json players;

                for (int i = 0; i < manager->objs.size(); i++) {
                    objs.push_back(manager->objs[i]->toJson());
                }

                for (int i = 0; i < manager->players.size(); i++) {
                    // if (players[i] == NULL) players.push_back(NULL);
                    players.push_back(manager->players[i]->toJson());
                }

                // std::cout << players << std::endl;

                // send message to client assigning ID = array index
                int id = manager->players.size() - 1;

                nlohmann::json all;
                all["id"] = id;
                all["objs"] = objs;
                all["players"] = players;

                manager->sock.send(zmq::buffer(all.dump()));

                int idx = manager->subs.size();
                manager->subs.push_back(zmq::socket_t(manager->context, zmq::socket_type::sub));
                std::stringstream ss;
                ss << "tcp://localhost:55" << std::setw(2) << std::setfill('0') << id;
                std::string connection = ss.str();
                // std::cout << connection << std::endl;
                manager->subs[idx].setsockopt(ZMQ_CONFLATE, 1);
                manager->subs[idx].connect(connection);
                const char * filter = "";
                manager->subs[idx].setsockopt(ZMQ_SUBSCRIBE, filter, strlen(filter));
            } else if (m.size() && (nlohmann::json::parse(m.to_string())["disconnecting"].is_boolean())) {
                int id = nlohmann::json::parse(m.to_string())["id"];
                (manager->players[id])->disconnect();
                manager->sock.send(zmq::buffer("disconnected"));
                manager->subs[id].close();
                manager->subs[id].~socket_t();

                events["events"].push_back(GameEvent(GameEvent::DISCONNECT, GameEvent::MEDIUM, manager->getGlobalTime(), manager->players[id], NULL).toJson());
                // manager->subs[id] = NULL;
            }

            std::vector<std::string> clientUpdates;

            for (int i = 0; i < manager->subs.size(); i++) {
                zmq::message_t temp;
                if (manager->subs[i]) manager->subs[i].recv(temp, zmq::recv_flags::dontwait);
                if (temp.size()) {
                    clientUpdates.push_back(temp.to_string());
                }
            }

            // loop through all client updates
            for (int i = 0; i < clientUpdates.size(); i++) {
                // std::cout << clientUpdates[i] << std::endl;

                nlohmann::json j = nlohmann::json::parse(clientUpdates[i]);

                // Making sure id field exists before pushing update
                if (!j["id"].is_null()) { 
                    int id = j["id"];
                    nlohmann::json p = j["player"];
                    std::string s = p.dump();
                    // printf("already existed\n");
                    // player already exists, update pos
                    // std::cout << "before update " << manager->players[j["id"]]->toJson() << std::endl;
                    manager->players[j["id"]]->doUpdate(j["player"]);
                    // std::cout << "after update " << manager->players[j["id"]]->toJson() << std::endl;
                    // if (j["id"] == 1) std::cout << j.dump() << std::endl;
                    // manager->sock.send(zmq::buffer(""));
                }     
            }

            nlohmann::json objs;
            nlohmann::json players;

            for (int i = 0; i < manager->objs.size(); i++) {
                objs.push_back(manager->objs[i]->toJson());
            }

            int end = 0;
            for (int i = 0; i < manager->players.size(); i++) {
                if (manager->players[i]) {
                    players.push_back(manager->players[i]->toJson());
                }
                else {
                    nlohmann::json j;
                    players.push_back(j);
                }
                end = i;
            }

            if (newChar) {
                players[end] = manager->players[end]->toJson();
            }

            nlohmann::json all;
            all["objs"] = objs;
            all["players"] = players;

            if (newChar) {
                GameEvent ev(GameEvent::JOIN, GameEvent::MEDIUM, manager->getGlobalTime(), manager->players[end], NULL);
                events["events"].push_back(ev.toJson());
            }

            manager->pub.send(zmq::buffer(all.dump()));
            manager->event_pub.send(zmq::buffer(events.dump()));

            // std::cout << players << std::endl;

            if (!c.isActive) {
                win->setActive(true);
                win->close();
            }
        }

        movement.join();
        render.join();
        // net.join();

        win->setActive(true);

        printf("shutting down\n");

        manager->shutdown();
    }

    return EXIT_SUCCESS;
}