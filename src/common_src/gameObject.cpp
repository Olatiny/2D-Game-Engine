#include "includes.hpp"
#include "gameObject.hpp"
#include "./components/components.hpp"

GameObject::GameObject() : RectangleShape(Vector2f(50, 50)) {
    setPosition(Vector2f(0, 0));
    setFillColor(Color::White);
    this->id = -1;

    topBox = RectangleShape(Vector2f(50, 10));
    topBox.setPosition(0, 0 - 10);
    bottomBox = RectangleShape(Vector2f(50, 10));
    bottomBox.setPosition(0, 0 + 50);
    leftBox = RectangleShape(Vector2f(10, 50));
    leftBox.setPosition(0 - 10, 0);
    rightBox = RectangleShape(Vector2f(10, 50));
    rightBox.setPosition(0 + 50, 0);

    velocity = Vector2f(0, 0);
}

GameObject::GameObject(int id) : RectangleShape(Vector2f(50, 50)) {
    setPosition(Vector2f(0, 0));
    setFillColor(Color::White);
    this->id = id;

    topBox = RectangleShape(Vector2f(50, 10));
    topBox.setPosition(0, 0 - 10);
    bottomBox = RectangleShape(Vector2f(50, 10));
    bottomBox.setPosition(0, 0 + 50);
    leftBox = RectangleShape(Vector2f(10, 50));
    leftBox.setPosition(0 - 10, 0);
    rightBox = RectangleShape(Vector2f(10, 50));
    rightBox.setPosition(0 + 50, 0);

    velocity = Vector2f(0, 0);
}

GameObject::GameObject(int id, Vector2f size, Vector2f pos, Texture *tex) : RectangleShape(size) {
    setPosition(pos);
    setTexture(tex);
    this->id = id;

    topBox = RectangleShape(Vector2f(size.x, 10));
    topBox.setPosition(pos.x, pos.y - 10);
    bottomBox = RectangleShape(Vector2f(size.x, 10));
    bottomBox.setPosition(pos.x, pos.y + size.y);
    leftBox = RectangleShape(Vector2f(10, size.y));
    leftBox.setPosition(pos.x - 10, pos.y);
    rightBox = RectangleShape(Vector2f(10, size.y));
    rightBox.setPosition(pos.x + size.x, pos.y);

    velocity = Vector2f(0, 0);
}

GameObject::GameObject(int id, Vector2f size, Vector2f pos, Color col) : RectangleShape(size) {
    setPosition(pos);
    setFillColor(col);
    this->id = id;

    topBox = RectangleShape(Vector2f(size.x, 10));
    topBox.setPosition(pos.x, pos.y - 10);
    bottomBox = RectangleShape(Vector2f(size.x, 10));
    bottomBox.setPosition(pos.x, pos.y + size.y);
    leftBox = RectangleShape(Vector2f(10, size.y));
    leftBox.setPosition(pos.x - 10, pos.y);
    rightBox = RectangleShape(Vector2f(10, size.y));
    rightBox.setPosition(pos.x + size.x, pos.y);

    velocity = Vector2f(0, 0);
}

void GameObject::setPosition(Vector2f pos) {
    RectangleShape::setPosition(pos);
    this->getTopBox()->setPosition(pos.x, pos.y - 10);
    this->getBottomBox()->setPosition(pos.x, pos.y + getSize().y);
    this->getLeftBox()->setPosition(pos.x - 10, pos.y);
    this->getRightBox()->setPosition(pos.x + getSize().x, pos.y);
}

void GameObject::move(Vector2f dist) {
    GameObject::setPosition(getPosition() + dist);
}

RectangleShape* GameObject::getTopBox() {
    return &(this->topBox);
}

RectangleShape* GameObject::getLeftBox() {
    return &(this->leftBox);
}

RectangleShape* GameObject::getBottomBox() {
    return &(this->bottomBox);
}

RectangleShape* GameObject::getRightBox() {
    return &(this->rightBox);
}

void GameObject::onEvent(GameEvent* ev, float dt) {
    std::unordered_map<Component::ComponentType, Component*>::iterator it;
    for (it = components.begin(); it != components.end(); it++) {
        it->second->onEvent(ev, dt);
    }
    // for (Component* c : components) {
    //     c->onEvent(ev, dt);
    // }
    
    // switch(e->type) {
    //     case GameEvent::COLLISION:
    //         // Component* c = getComponent(Component::COLLIDABLE); 
    //         break;
    //     case GameEvent::DEATH:
    //         break;
    //     case GameEvent::SPAWN:
    //         break;
    //     case GameEvent::INPUT:
    //         break;
    // }
}

void GameObject::doUpdate(std::vector<GameObject*>* objects, float dt) {
    std::unordered_map<Component::ComponentType, Component*>::iterator it;
    for (it = components.begin(); it != components.end(); it++) {
        it->second->update(*objects, dt);
    }
    // for (int i = 0; i < components.size(); i++) {
    //     components[i]->update(*objects, dt);
    // }
}

void GameObject::doUpdate(nlohmann::json j) {
    setPosition(Vector2f(j["position"][0], j["position"][1]));
    if (j["disconnected"].is_boolean()) {
        disconnected = j["disconnected"];
    }
}

void GameObject::addComponent(Component* c) {
    components[c->type] = c;
}

bool GameObject::hasComponent(Component::ComponentType t) {
    return components.find(t) != components.end();
    
    // for (int i = 0; i < components.size(); i++) {
    //     if ((components[i])->type == t) {
    //         return true;
    //     }
    // }

    // return false;
}

Component* GameObject::getComponent(Component::ComponentType t) {
    if (components.find(t) != components.end()) {
        return components[t];
    } else {
        return NULL;
    }

    // for (int i = 0; i < components.size(); i++) {
    //     if ((components[i])->type == t) {
    //         return components[i];
    //     }
    // }

    // return NULL;
}

nlohmann::json GameObject::toJson() {
    nlohmann::json j;
    
    j["id"] = id;
    j["disconnected"] = disconnected;
    j["size"] = {getSize().x, getSize().y};
    j["position"] = {getPosition().x, getPosition().y};
    if (!getTexture()) 
        j["fillColor"] = {getFillColor().r, getFillColor().g, getFillColor().b, getFillColor().a};
    else 
        j["fillColor"] = {0, 150, 255, 255/2};
    j["outlineColor"] = {getOutlineColor().r, getOutlineColor().g, getOutlineColor().b, getOutlineColor().a};
    j["outlineThickness"] = getOutlineThickness();
    j["components"];

    std::unordered_map<Component::ComponentType, Component*>::iterator it;
    for (it = components.begin(); it != components.end(); it++) {
        j["components"].push_back(it->second->toJson());
    }

    // for (Component* c : components) {
    //     j["components"].push_back(c->toJson());
    // }

    return j;
}

nlohmann::json GameObject::toJsonSmall() {
    using namespace nlohmann;

    json j;

    j["id"] = id;
    j["position"] = {getPosition().x, getPosition().y};
    j["disconnected"] = disconnected;

    return j;
}

GameObject::GameObject(nlohmann::json j) {
    this->id = j["id"];
    setSize(Vector2f(j["size"][0], j["size"][1]));
    setPosition(Vector2f(j["position"][0], j["position"][1]));
    setFillColor(Color(j["fillColor"][0], j["fillColor"][1], j["fillColor"][2], j["fillColor"][3]));
    setOutlineColor(Color(j["outlineColor"][0], j["outlineColor"][1], j["outlineColor"][2], j["outlineColor"][3]));
    setOutlineThickness(j["outlineThickness"]);
}

// void GameObject::draw(RenderTarget &target, RenderStates& states) {    
//     target.draw(*this, states);

//     std::cout << "drawing custom" << std::endl;

//     Component* c;
//     if (c = getComponent(Component::TEXT)) {
//         target.draw(*(dynamic_cast<TextComponent*>(c)->text), states);
//     }
// }

bool GameObject::isDisconnected() {
    return disconnected;
}

void GameObject::disconnect() {
    this->disconnected = true;
}

int GameObject::getId() {
    return this->id;
}

void GameObject::setId(int id) {
    this->id = id;
}

// /**
//  * Factory method for allowing javascript to create instances of native game
//  * objects
//  *
//  * NOTE: Like with the setters above, this static function does have a return
//  * type (and object), but the return value is placed in the function callback
//  * parameter, not the native c++ return type.
//  */
// void GameObject::ScriptedGameObjectFactory(const v8::FunctionCallbackInfo<v8::Value>& args)
// {
// 	v8::Isolate *isolate = args.GetIsolate();
// 	v8::Local<v8::Context> context = isolate->GetCurrentContext();
// 	v8::EscapableHandleScope handle_scope(args.GetIsolate());
// 	v8::Context::Scope context_scope(context);

// 	std::string context_name("default");
// 	if(args.Length() == 1)
// 	{
// 		v8::String::Utf8Value str(args.GetIsolate(), args[0]);
// 		context_name = std::string(v8helpers::ToCString(str));
// #if GO_DEBUG
// 		std::cout << "Created new object in context " << context_name << std::endl;
// #endif
// 	}
// 	GameObject *new_object = new GameObject();
// 	v8::Local<v8::Object> v8_obj = new_object->exposeToV8(isolate, context);
// 	args.GetReturnValue().Set(handle_scope.Escape(v8_obj));
// }

/**
 * IMPORTANT: Pay close attention to the definition of the std::vector in this
 * example implementation. The v8helpers::expostToV8 will assume you have
 * instantiated this exact type of vector and passed it in. If you don't the
 * helper function will not work. 
 */
v8::Local<v8::Object> GameObject::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
	std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
	v.push_back(v8helpers::ParamContainer("x", getGameObjectX, setGameObjectX));
	v.push_back(v8helpers::ParamContainer("y", getGameObjectY, setGameObjectY));
    v.push_back(v8helpers::ParamContainer("velocity_x", getGameObjectVelocityX, setGameObjectVelocityX));
    v.push_back(v8helpers::ParamContainer("velocity_y", getGameObjectVelocityY, setGameObjectVelocityY));
    v.push_back(v8helpers::ParamContainer("canJump", getGameObjectCanJump, setGameObjectCanJump));
	v.push_back(v8helpers::ParamContainer("guid", getGameObjectGUID, setGameObjectGUID));
	return v8helpers::exposeToV8(id, this, v, isolate, context, context_name);
}

/**
 * Implementations of static setter and getter functions
 *
 * IMPORTANT: These setter and getter functions will set and get values of v8
 * callback data structures. Note their return type is void regardless of
 * whether they are setter or getter. 
 *
 * Also keep in mind that the function signature must match this exactly in
 * order for v8 to accept these functions. 
 */ 

void GameObject::setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
    // std::cout << value->NumberValue() << std::endl;
	static_cast<GameObject*>(ptr)->setPosition(Vector2f(value->NumberValue(), static_cast<GameObject*>(ptr)->getPosition().y));
    // std::cout << static_cast<GameObject*>(ptr)->getPosition().x << " " << static_cast<GameObject*>(ptr)->getPosition().x << std::endl; 
}

void GameObject::getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	float x_val = static_cast<GameObject*>(ptr)->getPosition().x;
	info.GetReturnValue().Set(x_val);
}


void GameObject::setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	static_cast<GameObject*>(ptr)->setPosition(Vector2f(static_cast<GameObject*>(ptr)->getPosition().x, value->NumberValue()));
}

void GameObject::getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	float y_val = static_cast<GameObject*>(ptr)->getPosition().y;
	info.GetReturnValue().Set(y_val);
}

void GameObject::setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
	static_cast<GameObject*>(ptr)->id = std::stoi(*utf8_str);
}

void GameObject::getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	int guid = static_cast<GameObject*>(ptr)->id;
	v8::Local<v8::String> v8_guid = v8::String::NewFromUtf8(info.GetIsolate(), std::to_string(guid).c_str(), v8::String::kNormalString);
	info.GetReturnValue().Set(v8_guid);
}

void GameObject::setGameObjectVelocityX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    // std::cout << "setting velociy to: ";
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	// v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
    // v8::Local<v8::Array> arr = value.As<v8::Array>();
    // std::cout << arr->Get(0)->NumberValue() << "," << arr->Get(1)->NumberValue() << std::endl;
	static_cast<GameObject*>(ptr)->velocity.x = value->NumberValue();
    // std::cout << static_cast<GameObject*>(ptr)->velocity.x << "," << static_cast<GameObject*>(ptr)->velocity.y << std::endl;
}

void GameObject::getGameObjectVelocityX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	float vel_x = static_cast<GameObject*>(ptr)->velocity.x;
	v8::Local<v8::Number> vel = v8::Number::New(info.GetIsolate(), vel_x);
    // std::cout << "getting velocity" << std::endl;
	info.GetReturnValue().Set(vel);
}

void GameObject::setGameObjectVelocityY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    // std::cout << "setting velociy to: ";
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	// v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
    // v8::Local<v8::Array> arr = value.As<v8::Array>();
    // std::cout << arr->Get(0)->NumberValue() << "," << arr->Get(1)->NumberValue() << std::endl;
	static_cast<GameObject*>(ptr)->velocity.y = value->NumberValue();
    // std::cout << static_cast<GameObject*>(ptr)->velocity.x << "," << static_cast<GameObject*>(ptr)->velocity.y << std::endl;
}

void GameObject::getGameObjectVelocityY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	float vel_y = static_cast<GameObject*>(ptr)->velocity.y;
	v8::Local<v8::Number> vel = v8::Number::New(info.GetIsolate(), vel_y);
    // std::cout << "getting velocity" << std::endl;
	info.GetReturnValue().Set(vel);
}

void GameObject::setGameObjectCanJump(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    // std::cout << "setting velociy to: ";
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	// v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
    // v8::Local<v8::Array> arr = value.As<v8::Array>();
    // std::cout << arr->Get(0)->NumberValue() << "," << arr->Get(1)->NumberValue() << std::endl;
	static_cast<GameObject*>(ptr)->canJump = value->BooleanValue();
    // std::cout << static_cast<GameObject*>(ptr)->velocity.x << "," << static_cast<GameObject*>(ptr)->velocity.y << std::endl;
}

void GameObject::getGameObjectCanJump(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	bool cJump = static_cast<GameObject*>(ptr)->canJump;
	v8::Local<v8::Boolean> vel = v8::Boolean::New(info.GetIsolate(), cJump);
    // std::cout << "getting velocity" << std::endl;
	info.GetReturnValue().Set(vel);
}