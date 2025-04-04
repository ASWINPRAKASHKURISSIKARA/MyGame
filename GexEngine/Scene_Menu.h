#pragma once
#include "Scene.h"
#include "GameEngine.h"

class Scene_Menu : public Scene {
private:
    GameEngine* _game;
    sf::Texture _menuTexture;
    sf::Sprite _menuSprite;
    int _currentOption = 0;
    const int _optionCount = 3;

public:
    Scene_Menu(GameEngine* game);
    void update(sf::Time dt) override;
    void sRender() override;
    void doAction(const Command& command) override;
};
