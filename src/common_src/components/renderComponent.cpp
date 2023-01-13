#include "renderComponent.hpp"

RenderComponent::RenderComponent() {
    this->type = ComponentType::RENDER;
}

RenderComponent::RenderComponent(nlohmann::json j) {
    this->type = static_cast<ComponentType>(j["type"]);
}

void RenderComponent::update(std::vector<GameObject*> objects, float dt) {
    // does nothing
}

void RenderComponent::onEvent(GameEvent* ev, float dt) {

}

nlohmann::json RenderComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = type;

    return j;
}
