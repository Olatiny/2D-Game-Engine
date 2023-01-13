#include "alertComponent.hpp"
#include "textComponent.hpp"
#include <sys/types.h>

AlertComponent::AlertComponent(GameObject* obj, Timeline* tl) {
    this->obj = obj;
    this->tl = tl;
    this->type = ALERT;
}

AlertComponent::AlertComponent(GameObject* obj, Timeline* tl, nlohmann::json j) {

}

void AlertComponent::update(std::vector<GameObject*> objects, float dt) {

}

void AlertComponent::onEvent(GameEvent* ev, float dt) {
    if (ev->type == GameEvent::JOIN) {
        Component* c;
        if (c = obj->getComponent(TEXT)) {
            if (ev->raiseObj->hasComponent(PLAYER) || ev->raiseObj->getId() < 100) {
                dynamic_cast<TextComponent*>(c)->text->setString("player " + std::to_string(ev->raiseObj->getId()) + " joined.");
                // std::this_thread::sleep_for(std::chrono::seconds(1));
                // dynamic_cast<TextComponent*>(c)->text->setString("");
            }
        }
    }

    if (ev->type == GameEvent::DISCONNECT) {
        Component* c;
        if (c = obj->getComponent(TEXT)) {
            if (ev->raiseObj->hasComponent(PLAYER) || ev->raiseObj->getId() < 100) {
                dynamic_cast<TextComponent*>(c)->text->setString("player " + std::to_string(ev->raiseObj->getId()) + " disconnected.");
                // std::this_thread::sleep_for(std::chrono::seconds(1));
                // dynamic_cast<TextComponent*>(c)->text->setString("");
            }
        }
    }
    // if (ev->type == GameEvent::DEATH) {
    //     Component* c;
    //     if (c = obj->getComponent(TEXT)) {
    //         if (ev->raiseObj->hasComponent(PLAYER) || ev->raiseObj->getId() < 100) {
    //             dynamic_cast<TextComponent*>(c)->text->setString("player " + std::to_string(ev->raiseObj->getId()) + " died.");
    //             // std::this_thread::sleep_for(std::chrono::seconds(1));
    //             // dynamic_cast<TextComponent*>(c)->text->setString("");
    //         }
    //     }
    // }
}

nlohmann::json AlertComponent::toJson() {
    return NULL;
}