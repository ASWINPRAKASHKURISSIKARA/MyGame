#ifndef BREAKOUT_COMPONENTS_H
#define BREAKOUT_COMPONENTS_H


#include <memory>
#include <SFML/Graphics.hpp>
#include "Utilities.h"



struct Component
{
    bool		has{ false };
    Component() = default;
};


struct CShape : public Component
{
    std::shared_ptr<sf::Shape> shape{ nullptr };
    CShape() = default;

    CShape(std::shared_ptr < sf::Shape> shape, const sf::Color& fill, const sf::Color& outline=sf::Color::Black, float thickness = 5.f)
            : shape(shape)
    {
        shape->setFillColor(fill);
        shape->setOutlineColor(outline);
        shape->setOutlineThickness(thickness);
        centerOrigin(shape);
        has = true;
    }

};


struct CTransform : public Component
{

    sf::Transformable  tfm;
    sf::Vector2f	pos			{ 0.f, 0.f };
    sf::Vector2f	vel			{ 0.f, 0.f };
    sf::Vector2f	scale		{ 1.f, 1.f };

    float   angVel{ 0 };
    float	angle{ 0.f };

    CTransform() = default;
    CTransform(const sf::Vector2f& p) : pos(p)  {}
    CTransform(const sf::Vector2f& p, const sf::Vector2f& v)
            : pos(p),  vel(v){}

};

struct CName : public Component {
    sf::Text name;

    CName() = default;

    CName(const sf::Font& font, const std::string& n) {
        name.setFont(font);  
        name.setString(n);   
        name.setCharacterSize(20);  
        name.setFillColor(sf::Color::White);  
        centerOrigin(name);  
        has = true;  
    }
};


struct CCollision : public Component
{
    float radius{ 0.f };

    CCollision() = default;
    CCollision(float r)
            : radius(r) {}
};


struct CInput : public Component
{
    bool up{ false };
    bool left{ false };
    bool right{ false };
    bool down{ false };

    CInput() = default;
};

// Add these to your Components.h file

struct CDog : public Component {
    int health = 3;
    float distance = 0.0f;
    int boneCount = 0;
    int cookieCount = 0;
    int animationFrame = 0;
    bool isInvincible = false;
    float invincibilityTime = 0.0f;

    CDog() { has = true; }
};

struct CCar : public Component {
    bool goingDown = true;

    CCar() { has = true; }
    CCar(bool down) : goingDown(down) { has = true; }
};

struct CCollectible : public Component {
    enum Type { Bone, Cookie };
    Type type;

    CCollectible() { has = true; }
    CCollectible(Type t) : type(t) { has = true; }
};

struct CSprite : public Component {
    sf::Sprite sprite;

    CSprite() { has = true; }
    CSprite(const sf::Texture& texture) {
        sprite.setTexture(texture);
        has = true;
    }
};



#endif //BREAKOUT_COMPONENTS_H
