#pragma once

#include "includes.hpp"
#include "gameObject.hpp"
#include "timeline.hpp"

class GameObject;

class Component;

class GameEvent {
    public:
        enum EventType {COLLISION, DEATH, SPAWN, INPUT, TEXT, JOIN, DISCONNECT};
        enum EventPriority {HIGH, MEDIUM, LOW};
        
        const EventType type;
        const EventPriority priorityMod;
        GameObject* raiseObj;
        GameObject* withObj;
        nlohmann::json data;

        float priority;

        GameEvent(EventType type, EventPriority priority, Timeline* timeline, GameObject* raiseObj, GameObject* withObj);

        GameEvent(nlohmann::json j, GameObject* raiseObj, GameObject* withObj);

        void setData(nlohmann::json j);

        nlohmann::json toJson();
        
        /**
         * Overridden operator to sort priority queue properly.
         */
        bool operator< (GameEvent& otherEvent) {
            return priority < otherEvent.priority;
        }

        /**
         * Overridden operator to sort priority queue properly.
         */
        bool operator<= (GameEvent& otherEvent) {
            return priority <= otherEvent.priority;
        }

        /**
         * Overridden operator to sort priority queue properly.
         */
        bool operator= (GameEvent& otherEvent) {
            return priority == otherEvent.priority;
        }
        
        /**
         * Overridden operator to sort priority queue properly.
         */
        bool operator>= (GameEvent& otherEvent) {
            return priority >= otherEvent.priority;
        }

        /**
         * Overridden operator to sort priority queue properly.
         */
        bool operator> (GameEvent& otherEvent) {
            return priority > otherEvent.priority;
        }

        friend std::ostream& operator<< (std::ostream& os, GameEvent& ev) {
            return os << "{priority: " << ev.priority << ", type: " << ev.type << "}";
        }
};