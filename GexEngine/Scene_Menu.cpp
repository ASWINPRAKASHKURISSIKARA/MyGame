#include "Scene_Menu.h"
#include "Scene_Game.h"
#include "Assets.h"
#include <iostream>

Scene_Menu::Scene_Menu(GameEngine* game) : _game(game) {
    _actionMap[sf::Keyboard::Up] = "UP";
    _actionMap[sf::Keyboard::Down] = "DOWN";
    _actionMap[sf::Keyboard::Enter] = "SELECT";
    _actionMap[sf::Keyboard::Escape] = "EXIT";

    std::string assetsPath = "../assets/";

    if (!_menuTexture.loadFromFile(assetsPath + "menu.png")) {
        std::cerr << "Failed to load menu.png\n";
        exit(-1);
    }

    _menuSprite.setTexture(_menuTexture);
    _menuSprite.setScale(
        _game->windowSize().x / _menuTexture.getSize().x,
        _game->windowSize().y / _menuTexture.getSize().y
    );
}

void Scene_Menu::update(sf::Time dt) {
}

void Scene_Menu::sRender() {
    _game->window().draw(_menuSprite);

    sf::RectangleShape highlightRect;
    highlightRect.setSize(sf::Vector2f(400, 70));
    highlightRect.setFillColor(sf::Color(255, 255, 255, 100));

    if (_currentOption == 0) {
        highlightRect.setPosition(200, 400);
    }
    else if (_currentOption == 1) {
        highlightRect.setPosition(200, 500);
    }
    else if (_currentOption == 2) {
        highlightRect.setPosition(200, 600);
    }

    _game->window().draw(highlightRect);
}

void Scene_Menu::doAction(const Command& command) {
    if (command.getName() == "UP" && command.getType() == "START") {
        _currentOption = (_currentOption - 1 + _optionCount) % _optionCount;
    }
    else if (command.getName() == "DOWN" && command.getType() == "START") {
        _currentOption = (_currentOption + 1) % _optionCount;
    }
    else if (command.getName() == "SELECT" && command.getType() == "START") {
        if (_currentOption == 2) {
            _game->changeScene("GAME", std::make_shared<Scene_Game>(_game));
        }
    }
    else if (command.getName() == "EXIT" && command.getType() == "START") {
        _game->quit();
    }
}
