#pragma once

#include "../gameObject.hpp"
#include "component.hpp"
#include "../eventManager.hpp"
#include "../gameEvent.hpp"

class TextComponent : public Component {
    private:
        GameObject* obj;
        Timeline* tl;
        std::string font;
        Vector2f* objTranslation;
    public:
        Text* text;

        TextComponent(GameObject* obj, Timeline* tl, std::string str, sf::Font* font, std::string fontStr, int fontSize, Color col, Text::Style style, Vector2f pos);

        TextComponent(GameObject* obj, Timeline* tl, sf::Font* f, nlohmann::json j);

        void update(std::vector<GameObject*> objects, float dt = 0);

        void onEvent(GameEvent* ev, float dt = 0);

        nlohmann::json toJson();
};