//
// Created by David Burchill on 2023-09-27.
//

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

struct CName : public Component
{
    sf::T

    CName() = default;
    CName(sf::font font, std::string n) {
		name.setFont(font);
		nameText.setString(n);
        centerOrigin(name);
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


#endif //BREAKOUT_COMPONENTS_H
