#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Command.h"  

class GameEngine;

class Scene {
protected:
    std::map<sf::Keyboard::Key, std::string> _actionMap;

public:
    virtual ~Scene() = default;
    virtual void update(sf::Time dt) = 0;
    virtual void sRender() = 0;
    virtual void doAction(const Command& command) = 0;

    const std::map<sf::Keyboard::Key, std::string>& getActionMap() const { return _actionMap; }
};
