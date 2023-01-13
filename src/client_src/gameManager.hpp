#pragma once

#include "../common_src/includes.hpp"
#include "../common_src/gameEvent.hpp"
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
        sf::Font* f;
        float g;
        float last_physics_time;
        float physics_dt;
        float last_render_time;
        float render_dt;
        // float last_real_time;
    public:
        int id;
        zmq::context_t context;
        zmq::socket_t sock;
        zmq::socket_t pub;
        zmq::socket_t sub;
        // zmq::socket_t event_pub;
        zmq::socket_t event_sub;
        std::vector<GameObject*> objs;
        std::vector<GameObject*> players;
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

        static void RaiseV8(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void ScriptedGameObjectFactory(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void RotateFromScript(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void TextureFromScript(const v8::FunctionCallbackInfo<v8::Value>& args);

        void handleEvents();

        void updateObjects();

        GameObject* getObjectById(int id);

        void getServerUpdates();

        void getServerEvents();

        void sendToServer();

        void render();

        void shutdown();
};