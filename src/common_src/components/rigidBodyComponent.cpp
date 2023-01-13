#include "rigidBodyComponent.hpp"
#include "playerComponent.hpp"

RigidBodyComponent::RigidBodyComponent(GameObject* obj, Timeline* tl, std::mutex* _mutex, float g) {
    this->obj = obj;
    this->tl = tl;
    this->type = RIGID_BODY;
    this->g = g;
    this->_mutex = _mutex;
}

RigidBodyComponent::RigidBodyComponent(nlohmann::json j) {
    type = static_cast<ComponentType>(j["type"]);
}

void RigidBodyComponent::update(std::vector<GameObject*> objects, float dt) {
    // std::unique_lock<std::mutex> lock(*_mutex);

    // std::cout << 1 / dt << " fps" << std::endl;

    Vector2f *velocity = &(obj->velocity);

    if (g) {
        if (velocity->y <= g * 50) velocity->y += g;
    }

    // if (g) {
    //     // std::cout << "updated rigid body, g =  " << g << std::endl;
    //     if (dynamic_cast<PlayerComponent*>(obj->getComponent(PLAYER))->velocity.y <= g * 50) dynamic_cast<PlayerComponent*>(obj->getComponent(PLAYER))->velocity.y += g;
    // }
    
    // // dynamic_cast<PlayerComponent*>(obj->getComponent(PLAYER))->velocity.x * dt
    obj->move(Vector2f(0, velocity->y * dt));
}

void RigidBodyComponent::onEvent(GameEvent* ev, float dt) {
    if (ev->type == GameEvent::COLLISION && ev->withObj == obj) {
        // std::cout << "yarr" << std::endl;
        bool grounded = false;

        FloatRect this_bounds = FloatRect(obj->getGlobalBounds());
        FloatRect other_bounds = FloatRect(ev->raiseObj->getGlobalBounds());

        // pattern: this_to_other
        float left_to_right = (other_bounds.left + other_bounds.width) - this_bounds.left;
        float right_to_left = this_bounds.left + this_bounds.width - other_bounds.left;
        float top_to_bottom = (other_bounds.top + other_bounds.height) - this_bounds.top;
        float bottom_to_top = this_bounds.top + this_bounds.height - other_bounds.top;

        float min = std::min({std::abs(left_to_right), std::abs(right_to_left), std::abs(top_to_bottom), std::abs(bottom_to_top)});

        // if (!this_bounds.intersects(other_bounds)) continue;
        Vector2f *velocity = &(obj->velocity);
        bool *canJump = &(obj->canJump);

        std::unique_lock<std::mutex> lock(*(_mutex));

        if (min == left_to_right) {
            if (velocity->x <= 0) {
                obj->setPosition(Vector2f(other_bounds.left + other_bounds.width, this_bounds.top));
                velocity->x = 0;
            }
        } else if (min == right_to_left) {
            if (velocity->x >= 0) {
                obj->setPosition(Vector2f(other_bounds.left - this_bounds.width, this_bounds.top));
                velocity->x = 0;
            }
        } else if (min == top_to_bottom) {
            if (velocity->y < 0) {
                obj->setPosition(Vector2f(this_bounds.left, other_bounds.top + other_bounds.height));
                velocity->y = 0;
            }
        } else if (min == bottom_to_top) {
            // noCollision = false;
            // printf("here\n");
            if (velocity->y > 0) {
                *canJump = true;
                obj->setPosition(Vector2f(this_bounds.left, other_bounds.top - this_bounds.height));
                velocity->y = 0;
            }
        }

        if (obj->getBottomBox()->getGlobalBounds().intersects(other_bounds)) {
            *canJump = true;
            grounded = true;
        }
        
        if (!grounded) {
            // std::unique_lock<std::mutex> lock(*(_mutex));
            // printf("hi\n");
            *canJump = false;
        }
    }
}

nlohmann::json RigidBodyComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;

    return j;
}