#include "Scene_Title.h"
#include "Scene_Menu.h"
#include "Assets.h"
#include <iostream>

Scene_Title::Scene_Title(GameEngine* game) : _game(game) {
    _actionMap[sf::Keyboard::Enter] = "START";
    _actionMap[sf::Keyboard::Escape] = "EXIT";

    std::string assetsPath = "../assets/";

    if (!_titleTexture.loadFromFile(assetsPath + "title.png")) {
        std::cerr << "Failed to load title.png\n";
        exit(-1);
    }

    _titleSprite.setTexture(_titleTexture);
    _titleSprite.setScale(
        _game->windowSize().x / _titleTexture.getSize().x,
        _game->windowSize().y / _titleTexture.getSize().y
    );
}

void Scene_Title::update(sf::Time dt) {
}

void Scene_Title::sRender() {
    _game->window().draw(_titleSprite);
}

void Scene_Title::doAction(const Command& command) {
    if (command.getName() == "START" && command.getType() == "START") {
        _game->changeScene("MENU", std::make_shared<Scene_Menu>(_game));
    }
    else if (command.getName() == "EXIT" && command.getType() == "START") {
        _game->quit();
    }
}
