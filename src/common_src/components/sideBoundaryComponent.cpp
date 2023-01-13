#include "sideBoundaryComponent.hpp"

SideBoundaryComponent::SideBoundaryComponent(GameObject* obj, Timeline* tl, RenderWindow* window, int player_id) {
    this->window = window;
    this->player_id = player_id;
    this->type = SCREEN_TRANSITION;
    this->obj = obj;
    this->tl = tl;
}

void SideBoundaryComponent::update(std::vector<GameObject*> objects, float dt) {
    // if (player_id < 0) {
    //     return;
    // }

    // if (obj->getGlobalBounds().intersects(objects[player_id]->getGlobalBounds())) {
    //     View tempv = window->getView();

    //     if (objects[player_id]->getRightBox()->getGlobalBounds().intersects(obj->getGlobalBounds())) {
    //         tempv.setCenter(Vector2f(obj->getPosition().x - (1280 / 2), tempv.getCenter().y));
    //     } else if (objects[player_id]->getLeftBox()->getGlobalBounds().intersects(obj->getGlobalBounds())) {
    //         tempv.setCenter(Vector2f(obj->getPosition().x + (1280 / 2), tempv.getCenter().y));
    //     }

    //     window->setView(tempv);
    // }
}

void SideBoundaryComponent::onEvent(GameEvent* ev, float dt) {
    if (player_id < 0) {
        return;
    }

    if (ev->type == GameEvent::COLLISION) {
        if (ev->withObj == obj && ev->raiseObj->getId() == player_id) {
            // std::cout << "side boundary collision" << std::endl;
            View tempv = window->getView();

            if (ev->raiseObj->getRightBox()->getGlobalBounds().intersects(obj->getGlobalBounds())) {
                tempv.setCenter(Vector2f(obj->getPosition().x - (1280 / 2), tempv.getCenter().y));
            } else if (ev->raiseObj->getLeftBox()->getGlobalBounds().intersects(obj->getGlobalBounds())) {
                tempv.setCenter(Vector2f(obj->getPosition().x + (1280 / 2), tempv.getCenter().y));
            }

            window->setView(tempv);
        }
    }
}

nlohmann::json SideBoundaryComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = SCREEN_TRANSITION;

    return j;
}