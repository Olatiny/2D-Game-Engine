// #include <v8.h>
#include "../common_src/includes.hpp"
#include "gameManager.hpp"

nlohmann::json lastKeys;

void checkKeysPressed() {
    using namespace nlohmann;

    json keys;

    bool raiseEvent = false;

    for (int i = -1; i < sf::Keyboard::KeyCount; i++) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))) {
            if (!raiseEvent) raiseEvent = true;

            if (Keyboard::isKeyPressed(Keyboard::A) && lastKeys["A"].is_null()) keys["A"] = true; else keys["A"] = false;
            if (Keyboard::isKeyPressed(Keyboard::B) && lastKeys["B"].is_null()) keys["B"] = true; else keys["B"] = false;
            if (Keyboard::isKeyPressed(Keyboard::C) && lastKeys["C"].is_null()) keys["C"] = true; else keys["C"] = false;
            if (Keyboard::isKeyPressed(Keyboard::D) && lastKeys["D"].is_null()) keys["D"] = true; else keys["D"] = false;
            if (Keyboard::isKeyPressed(Keyboard::E) && lastKeys["E"].is_null()) keys["E"] = true; else keys["E"] = false;
            if (Keyboard::isKeyPressed(Keyboard::F) && lastKeys["F"].is_null()) keys["F"] = true; else keys["F"] = false;
            if (Keyboard::isKeyPressed(Keyboard::G) && lastKeys["G"].is_null()) keys["G"] = true; else keys["G"] = false;
            if (Keyboard::isKeyPressed(Keyboard::H) && lastKeys["H"].is_null()) keys["H"] = true; else keys["H"] = false;
            if (Keyboard::isKeyPressed(Keyboard::I) && lastKeys["I"].is_null()) keys["I"] = true; else keys["I"] = false;
            if (Keyboard::isKeyPressed(Keyboard::J) && lastKeys["J"].is_null()) keys["J"] = true; else keys["J"] = false;
            if (Keyboard::isKeyPressed(Keyboard::K) && lastKeys["K"].is_null()) keys["K"] = true; else keys["K"] = false;
            if (Keyboard::isKeyPressed(Keyboard::L) && lastKeys["L"].is_null()) keys["L"] = true; else keys["L"] = false;
            if (Keyboard::isKeyPressed(Keyboard::M) && lastKeys["M"].is_null()) keys["M"] = true; else keys["M"] = false;
            if (Keyboard::isKeyPressed(Keyboard::N) && lastKeys["N"].is_null()) keys["N"] = true; else keys["N"] = false;
            if (Keyboard::isKeyPressed(Keyboard::O) && lastKeys["O"].is_null()) keys["O"] = true; else keys["O"] = false;
            if (Keyboard::isKeyPressed(Keyboard::P) && lastKeys["P"].is_null()) keys["P"] = true; else keys["P"] = false;
            if (Keyboard::isKeyPressed(Keyboard::Q) && lastKeys["Q"].is_null()) keys["Q"] = true; else keys["Q"] = false;
            if (Keyboard::isKeyPressed(Keyboard::R) && lastKeys["R"].is_null()) keys["R"] = true; else keys["R"] = false;
            if (Keyboard::isKeyPressed(Keyboard::S) && lastKeys["S"].is_null()) keys["S"] = true; else keys["S"] = false;
            if (Keyboard::isKeyPressed(Keyboard::T) && lastKeys["T"].is_null()) keys["T"] = true; else keys["T"] = false;
            if (Keyboard::isKeyPressed(Keyboard::U) && lastKeys["U"].is_null()) keys["U"] = true; else keys["U"] = false;
            if (Keyboard::isKeyPressed(Keyboard::V) && lastKeys["V"].is_null()) keys["V"] = true; else keys["V"] = false;
            if (Keyboard::isKeyPressed(Keyboard::W) && lastKeys["W"].is_null()) keys["W"] = true; else keys["W"] = false;
            if (Keyboard::isKeyPressed(Keyboard::X) && lastKeys["X"].is_null()) keys["X"] = true; else keys["X"] = false;
            if (Keyboard::isKeyPressed(Keyboard::Y) && lastKeys["Y"].is_null()) keys["Y"] = true; else keys["Y"] = false;
            if (Keyboard::isKeyPressed(Keyboard::Z) && lastKeys["Z"].is_null()) keys["Z"] = true; else keys["Z"] = false;
            if (Keyboard::isKeyPressed(Keyboard::LShift) && lastKeys["LShift"].is_null()) keys["LShift"] = true; else keys["LShift"] = false;
            if (Keyboard::isKeyPressed(Keyboard::Space) && lastKeys["Space"].is_null()) keys["Space"] = true; else keys["Space"] = false;
            if (Keyboard::isKeyPressed(Keyboard::Left) && lastKeys["Left"].is_null()) keys["Left"] = true; else keys["Left"] = false;
            if (Keyboard::isKeyPressed(Keyboard::Up) && lastKeys["Up"].is_null()) keys["Up"] = true; else keys["Up"] = false;
            if (Keyboard::isKeyPressed(Keyboard::Down) && lastKeys["Down"].is_null()) keys["Down"] = true; else keys["Down"] = false;
            if (Keyboard::isKeyPressed(Keyboard::Right) && lastKeys["Right"].is_null()) keys["Right"] = true; else keys["Right"] = false;

        }
    }
    
    json j;

    lastKeys = j;

    j["keys"] = keys;

    if (raiseEvent) {
        GameEvent* ev = new GameEvent(GameEvent::INPUT, GameEvent::HIGH, GameManager::getInstance()->getGlobalTime(), GameManager::getInstance()->players[GameManager::getInstance()->id], NULL);
        ev->data = j;

        EventManager::getInstance()->raise(ev);
    }
}

class GameThread {
    public:
        GameManager* manager;
        v8::Isolate* isolate;
        // std::condition_variable* _condition_variable;

        GameThread(GameManager* manager, v8::Isolate* isolate/*, std::condition_variable* _condition_variable*/) {
            this->manager = manager;
            this->isolate = isolate;
        }

        virtual void run() = 0;
};

class NetworkThread : GameThread {
    public:
        using GameThread::GameThread;

        void run() {            
            while (manager->getWindow()->isOpen()) {
                manager->getServerUpdates();

                manager->getServerEvents();

                manager->sendToServer(); 
            }
        }
};

class PhysicsThread : GameThread {
    public:
        using GameThread::GameThread;

        void run() {
            while (this->manager->getWindow()->isOpen()) {
                // printf("physics\n");
                // manager->handleEvents();
                // if (this->manager->getWindow()->hasFocus()) {
                //     checkKeysPressed();
                // }
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
                this->manager->render();

                if (canClose){
                    this->manager->getWindow()->setActive(false);
                    isActive = false;
                    break;
                }
            }
        }

        void closeWindow() {
            canClose = true;
        }
};

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper(NetworkThread *fe)
{
    fe->run();
}

void physics_wrapper(PhysicsThread *fe) {
    fe->run();
}

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper2(RenderThread *fe)
{
    fe->run();
}

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

    // { // scope for V8
    //     v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
    //     v8::HandleScope handle_scope(isolate);

    //     v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

    //     // Bind the global 'print' function to the C++ Print callback.
    //     global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
	// 	// Bind the global static factory function for creating new GameObject instances
	// 	// global->Set(isolate, "gameobjectfactory", v8::FunctionTemplate::New(isolate, GameObject::ScriptedGameObjectFactory));
	// 	// Bind the global static function for retrieving object handles
	// 	global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
    // }


    {   
        v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
        v8::HandleScope handle_scope(isolate);

        manager->startup(isolate);

        RenderWindow* win = manager->getWindow();
        const View originalView = win->getView();
        View view = win->getView();

        // win->setActive(false);

        PhysicsThread p(manager, isolate);
        // RenderThread c(manager);
        NetworkThread n(manager, isolate);

        std::thread movement(physics_wrapper, &p);
        // std::thread render(run_wrapper2, &c);
        std::thread network(run_wrapper, &n);

        while (win->isOpen()) {
            // Timeline t(manager->getGlobalTime(), 1);

            // check for input
            if (win->hasFocus()) {
                checkKeysPressed();
            }

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

            // float dt;
            // while (dt = t.getTime() < 1/120.f);
            manager->updateObjects();

            manager->handleEvents();

            // if (win->hasFocus()) {
            //     win->setActive(true);
            //     // while (t.getTime() < 1/60.f);
            manager->render();
            // } else {
            //     win->setActive(false);
            // }

            // if (!c.isActive) {
            //     win->setActive(true);
            //     win->close();
            // }
        }

        movement.join();
        // render.join();
        network.join();

        // win->setActive(true);

        manager->shutdown();
    }

    return EXIT_SUCCESS;
}