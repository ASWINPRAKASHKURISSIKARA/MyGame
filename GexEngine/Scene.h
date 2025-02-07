#pragma once
#ifndef BREAKOUT_SCENE_H
#define BREAKOUT_SCENE_H

#include <SFML/Graphics.hpp>

class Scene {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    Scene(const std::string& backgroundPath);

    void render(sf::RenderWindow& window);
};

#endif //BREAKOUT_SCENE_H
