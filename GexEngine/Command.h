#pragma once
#include <string>
#include <SFML/System/Time.hpp>

class Command {
private:
    std::string _name;
    std::string _type;  
    sf::Time _dt;

public:
    Command(const std::string& name, const std::string& type)
        : _name(name), _type(type), _dt(sf::Time::Zero) {}

    Command(const std::string& name, sf::Time dt)
        : _name(name), _type("START"), _dt(dt) {}

    const std::string& getName() const { return _name; }
    const std::string& getType() const { return _type; }
    sf::Time getDeltaTime() const { return _dt; }
};
