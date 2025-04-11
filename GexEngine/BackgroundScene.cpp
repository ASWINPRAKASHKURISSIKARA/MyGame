#include "BackgroundScene.h"
#include <iostream>

BackgroundScene::BackgroundScene(const std::string& backgroundPath) {
    std::string fullPath = "../assets/" + backgroundPath;

    if (!backgroundTexture.loadFromFile(fullPath)) {
        std::cerr << "Failed to load background image: " << fullPath << "\n";
    }
    else {
        backgroundSprite.setTexture(backgroundTexture);

        sf::Vector2u windowSize = sf::Vector2u(1280, 768);

        sf::Vector2u textureSize = backgroundTexture.getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

        backgroundSprite.setScale(scaleX, scaleY);
    }
}

void BackgroundScene::sRender() {
}

void BackgroundScene::render(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
}
