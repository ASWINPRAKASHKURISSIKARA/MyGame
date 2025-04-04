#pragma once
#include "Scene.h"
#include "GameEngine.h"

class Scene_Title : public Scene {
private:
    GameEngine* _game;
    sf::Texture _titleTexture;
    sf::Sprite _titleSprite;

public:
    Scene_Title(GameEngine* game);
    void update(sf::Time dt) override;
    void sRender() override;
    void doAction(const Command& command) override;
};
