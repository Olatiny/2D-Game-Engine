#include "gameEvent.hpp"

GameEvent::GameEvent(EventType eType, EventPriority ePriority, Timeline* timeline, GameObject* raise, GameObject* with) : type(eType), priorityMod(ePriority), raiseObj(raise), withObj(with) {
    priority = timeline->getTime() * priorityMod;
};

GameEvent::GameEvent(nlohmann::json j, GameObject* raiseObj, GameObject* withObj) : type(j["type"]), priorityMod(static_cast<EventPriority>(j["priorityMod"])) {
    using namespace nlohmann;

    priority = j["priority"];
    this->raiseObj = raiseObj;
    this->withObj = withObj;
    data = j["data"];
}

void GameEvent::setData(nlohmann::json j) {
    this->data = j;
}

nlohmann::json GameEvent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;
    j["priority"] = priority;
    j["priorityMod"] = priorityMod;
    j["raiseObjID"] = raiseObj->getId();
    j["withObjID"] = withObj ? withObj->getId() : -1;
    j["data"] = data;

    return j;
}