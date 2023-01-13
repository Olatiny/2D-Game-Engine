#pragma once

#include "includes.hpp"
#include "gameEvent.hpp"
#include "./components/component.hpp"

class GameEvent;

class GameObject : public RectangleShape {
    private:
        int id;
        bool disconnected = false;
        RectangleShape topBox;
        RectangleShape bottomBox;
        RectangleShape leftBox;
        RectangleShape rightBox;
    public:
        std::unordered_map<Component::ComponentType, Component*> components;
        v8::Context *context;
        Vector2f velocity;
        bool canJump;

        // std::vector<Component*> components;

        GameObject();

        GameObject(int id);

        GameObject(int id, Vector2f size, Vector2f pos, Texture *tex);

        GameObject(int id, Vector2f size, Vector2f pos, Color col);

        GameObject(nlohmann::json j);

        int getId();

        void setId(int id);

        void doUpdate(std::vector<GameObject*>* objs, float dt);

        void doUpdate(nlohmann::json j);

        void onEvent(GameEvent* ev, float dt = 0);

        // void draw(RenderTarget &target, RenderStates& states);

        // void addComponent();

        void addComponent(Component* c);

        bool hasComponent(Component::ComponentType t);

        Component* getComponent(Component::ComponentType t);

        void setPosition(Vector2f pos);

        void move(Vector2f dist);

        RectangleShape* getTopBox();

        RectangleShape* getBottomBox();

        RectangleShape* getLeftBox();

        RectangleShape* getRightBox();

        void disconnect();

        bool isDisconnected();

        nlohmann::json toJson();

        nlohmann::json toJsonSmall();

		v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");

        static void setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

        static void getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

        static void getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

        static void getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void setGameObjectVelocityX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

        static void getGameObjectVelocityX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        
        static void setGameObjectVelocityY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

        static void getGameObjectVelocityY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        static void setGameObjectCanJump(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

        static void getGameObjectCanJump(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
};