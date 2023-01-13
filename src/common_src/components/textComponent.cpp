#include "textComponent.hpp"

TextComponent::TextComponent(GameObject* obj, Timeline* tl, std::string str, sf::Font* font, std::string fontStr, int fontSize, Color col, Text::Style style, Vector2f pos) {
    // sf::Font* temp_font = new sf::Font();
    this->type = TEXT;
    this->font = fontStr;
    this->text = new Text();
    
    this->obj = obj;
    this->tl = tl;

    // temp_font->loadFromFile(font);

    // std::cout << str << std::endl;
    text->setString(str.c_str());
    text->setCharacterSize(fontSize);
    text->setFillColor(col);
    text->setStyle(style);
    text->setPosition(pos);
    text->setFont(*font);
    objTranslation = new Vector2f(obj->getPosition() - pos);
}

TextComponent::TextComponent(GameObject* obj, Timeline* tl, sf::Font* f, nlohmann::json j) {
    this->obj = obj;
    this->tl = tl;

    this->type = TEXT;
    this->text = new Text();
    text->setFont(*f);
    
    this->obj = obj;
    this->tl = tl;

    this->font = j["font"];
    text->setString(std::string(j["string"]));
    text->setCharacterSize(j["fontSize"]);
    text->setFillColor(Color(j["color"][0],j["color"][1], j["color"][2], j["color"][3]));
    text->setStyle(j["style"]);
    this->objTranslation = new Vector2f(j["translation"][0], j["translation"][1]);
}


void TextComponent::update(std::vector<GameObject*> objects, float dt) {
    // Vector2f temp(obj->getPosition().x, obj->getPosition().y);
    text->setPosition(obj->getPosition() - *objTranslation);
}

void TextComponent::onEvent(GameEvent* ev, float dt) {
    if (ev->type == GameEvent::TEXT) {
        if (ev->withObj == obj) {
            text->setString(std::string(ev->data["string"]));
        }
    }
}

nlohmann::json TextComponent::toJson() {
    using namespace nlohmann;

    json j;

    j["type"] = TEXT;
    j["string"] = text->getString();
    j["font"] = font;
    j["fontSize"] = text->getCharacterSize();
    j["color"] = {text->getFillColor().r, text->getFillColor().g, text->getFillColor().b, text->getFillColor().a};
    j["style"] = text->getStyle();
    j["translation"] = {objTranslation->x, objTranslation->y};

    return j;
}