#pragma once

#include "../common_src/includes.hpp"
#include "../common_src/gameObject.hpp"
#include "../common_src/components/components.hpp"
#include "../common_src/timeline.hpp"

class GameManager {
    private:
        GameManager() {};
        RenderWindow* w;
        Timeline* globalTime;
        Timeline* physicsTime;
        Timeline* renderTime;
        std::mutex _mutex;
        std::condition_variable _condition_variable;
        float g;
        float last_physics_time;
        float last_render_time;
    public:
        sf::Font* f;
        std::vector<GameObject*> objs;
        std::vector<GameObject*> players;
        zmq::context_t context;
        zmq::socket_t sock;
        std::vector<zmq::socket_t> subs;
        zmq::socket_t pub;
        zmq::socket_t event_pub;
        v8::Isolate *isolate;
        ScriptManager *sm;

        GameManager(GameManager &other) = delete;

        void operator = (const GameManager &) = delete;

        static GameManager* getInstance();

        RenderWindow* getWindow();

        std::mutex* getMutex();

        Timeline* getGlobalTime();

        void setGravity(float g);

        float getGravity();

        void startup(v8::Isolate *isolate);

        void updateObjects();

        void render();

        void shutdown();
};