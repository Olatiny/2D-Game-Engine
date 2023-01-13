#include "playerComponent.hpp"
#include "rigidBodyComponent.hpp"

PlayerComponent::PlayerComponent(GameObject* obj, Timeline* tl, Vector2f velocity, float speed, float gravity, bool canJump, RenderWindow* win, std::mutex* _mutex) {
    this->obj = obj;
    this->tl = tl;
    
    this->type = ComponentType::PLAYER;
    this->velocity = velocity;
    this->speed = speed;
    this->gravity = gravity;
    this->canJump = canJump;
    this->window = win;
    
    this->_mutex = _mutex;
}

PlayerComponent::PlayerComponent(nlohmann::json j) {
    type = static_cast<ComponentType>(j["type"]);
    velocity = Vector2f(j["velocity"][0], j["velocity"][1]);
    speed = j["speed"];
    gravity = j["gravity"];
    canJump = j["canJump"];
}

void PlayerComponent::update(std::vector<GameObject*> objects, float dt) {
    // doMovement(obj, dt);
    if (gravity) {
        if (velocity.y <= gravity * 50) velocity.y += gravity;
    }
    obj->move(Vector2f(velocity.x * dt, velocity.y * dt));
    // doCollision(obj, objects);
}

void PlayerComponent::onEvent(GameEvent* ev, float dt) {
    if (ev->type == GameEvent::INPUT) {
        doMovement(obj, dt);
        // dynamic_cast<RigidBodyComponent*>(obj->getComponent(RIGID_BODY))->onEvent(new GameEvent(GameEvent::COLLISION))
    }  else {
        velocity.x = 0;
        if (!gravity) velocity.y = 0;
    }

    if (ev->type == GameEvent::COLLISION) {
        // std::cout << "rise id " << ev->raiseObj->getId() << std::endl;
        // std::cout << "with id " << ev->withObj->getId() << std::endl;
        // std::cout << "obj id " << obj->getId() << std::endl;
    }

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

        std::unique_lock<std::mutex> lock(*(_mutex));

        // std::cout << "locking" << std::endl;

        if (min == left_to_right) {
            if (velocity.x <= 0) {
                obj->setPosition(Vector2f(other_bounds.left + other_bounds.width, this_bounds.top));
                velocity.x = 0;
            }
        } else if (min == right_to_left) {
            if (velocity.x >= 0) {
                obj->setPosition(Vector2f(other_bounds.left - this_bounds.width, this_bounds.top));
                velocity.x = 0;
            }
        } else if (min == top_to_bottom) {
            if (velocity.y < 0) {
                obj->setPosition(Vector2f(this_bounds.left, other_bounds.top + other_bounds.height));
                velocity.y = 0;
            }
        } else if (min == bottom_to_top) {
            // noCollision = false;
            // printf("here\n");
            if (velocity.y > 0) {
                canJump = true;
                obj->setPosition(Vector2f(this_bounds.left, other_bounds.top - this_bounds.height));
                velocity.y = 0;
            }
        }

        if (obj->getBottomBox()->getGlobalBounds().intersects(other_bounds)) {
            canJump = true;
            grounded = true;
        }
        
        if (!grounded) {
            // std::unique_lock<std::mutex> lock(*(_mutex));
            // printf("hi\n");
            canJump = false;
        }
    }
    // velocity.x = 0;
    // if (!gravity) velocity.y = 0;
    
}

void PlayerComponent::doMovement(GameObject* obj, float dt) {
    std::unique_lock<std::mutex> lock(*(_mutex));

    float lspeed = Keyboard::isKeyPressed(Keyboard::LShift) ? speed * 2.f : speed;

    // bool left = false, right = false, up = false, down = true;

    if (!dt) return;

    velocity.x = 0;

    if (!gravity) velocity.y = 0;

    // printf("%lf\n", velocity.y);

    if (window->hasFocus()) {
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
            velocity += Vector2f(-lspeed, 0);
            // left = true;
        }
        
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
            velocity += Vector2f(lspeed, 0);
            // right = true;
        }

        if ((gravity ? this->canJump && velocity.y >= 0 : 1) && (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Space))) {
            velocity += Vector2f(0, (gravity ? -(50 * gravity) : -lspeed));
            canJump = false;
            // up = true;
        }
    }
    
    if (gravity) {
        // if (velocity.y <= gravity * 50) velocity.y += gravity;
    } else {
        if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
            velocity += Vector2f(0, lspeed);
            // down = true;
            // printf("here\n");
        }
    }

    // obj->move(Vector2f(velocity.x * dt, velocity.y * dt));
}

void PlayerComponent::doCollision(GameObject* obj, std::vector<GameObject*> objects) {
    bool grounded = false;

        // printf("%d\n", this);
    for (int i = 0; i < objects.size(); i++) {
        // std::cout << "rawr " << i << " " << objects.at(i)->hasComponent(COLLIDABLE) << std::endl;
        if ((!objects.at(i)->hasComponent(COLLIDABLE)) || (objects.at(i) == obj)) continue;
        // std::cout << "yarr" << std::endl;

        FloatRect this_bounds = FloatRect(obj->getGlobalBounds());
        FloatRect other_bounds = FloatRect(objects.at(i)->getGlobalBounds());

        // pattern: this_to_other
        float left_to_right = (other_bounds.left + other_bounds.width) - this_bounds.left;
        float right_to_left = this_bounds.left + this_bounds.width - other_bounds.left;
        float top_to_bottom = (other_bounds.top + other_bounds.height) - this_bounds.top;
        float bottom_to_top = this_bounds.top + this_bounds.height - other_bounds.top;

        float min = std::min({std::abs(left_to_right), std::abs(right_to_left), std::abs(top_to_bottom), std::abs(bottom_to_top)});

        if (!this_bounds.intersects(other_bounds)) continue;

        std::unique_lock<std::mutex> lock(*(_mutex));

        if (min == left_to_right) {
            if (velocity.x < 0) {
                obj->setPosition(Vector2f(other_bounds.left + other_bounds.width, this_bounds.top));
                velocity.x = 0;
            }
        } else if (min == right_to_left) {
            if (velocity.x > 0) {
                obj->setPosition(Vector2f(other_bounds.left - this_bounds.width, this_bounds.top));
                velocity.x = 0;
            }
        } else if (min == top_to_bottom) {
            if (velocity.y < 0) {
                obj->setPosition(Vector2f(this_bounds.left, other_bounds.top + other_bounds.height));
                velocity.y = 0;
            }
        } else if (min == bottom_to_top) {
            // noCollision = false;
            // printf("here\n");
            if (velocity.y > 0) {
                canJump = true;
                obj->setPosition(Vector2f(this_bounds.left, other_bounds.top - this_bounds.height));
                velocity.y = 0;
            }
        }

        if (obj->getBottomBox()->getGlobalBounds().intersects(other_bounds)) {
            canJump = true;
            grounded = true;
        }
    }

    if (!grounded) {
        std::unique_lock<std::mutex> lock(*(_mutex));
        // printf("hi\n");
        canJump = false;
    }
}

nlohmann::json PlayerComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;
    j["velocity"] = {velocity.x, velocity.y};
    j["speed"] = speed;
    j["gravity"] = gravity;
    j["canJump"] = canJump;

    return j;
}