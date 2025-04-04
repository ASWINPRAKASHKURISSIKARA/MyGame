#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>

class BackgroundScene : public Scene {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    BackgroundScene(const std::string& backgroundPath);

    void update(sf::Time dt) override {}
    void sRender() override;
    void doAction(const Command& command) override {}

    
    void render(sf::RenderWindow& window);
};
