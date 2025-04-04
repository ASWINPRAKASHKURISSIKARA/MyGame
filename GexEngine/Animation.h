#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class Animation {
private:
    std::string _name;
    std::vector<sf::IntRect> _frames;
    sf::Time _timePerFrame;
    bool _isRepeating;
    sf::Time _countDown;
    sf::Sprite _sprite;
    size_t _currentFrame{ 0 };

public:
    Animation(const std::string& name,
        const sf::Texture& t,
        std::vector<sf::IntRect> frames,
        sf::Time tpf,
        bool repeats);

    void update(sf::Time dt);
    bool hasEnded() const;
    const std::string& getName() const;
    sf::Sprite& getSprite();
    sf::Vector2f getBB() const;
};

#endif // ANIMATION_H
