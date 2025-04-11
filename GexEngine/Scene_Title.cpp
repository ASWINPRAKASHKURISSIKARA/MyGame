#include "Scene_Title.h"
#include "Scene_Menu.h"
#include "Assets.h"
#include <iostream>

Scene_Title::Scene_Title(GameEngine* game) : _game(game) {
    _actionMap[sf::Keyboard::Enter] = "START";
    _actionMap[sf::Keyboard::Escape] = "EXIT";

    std::string assetsPath = "../assets/";

    _titleTexture = Assets::getInstance().getTexture("title");
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

    sf::Text enterText;
    enterText.setFont(Assets::getInstance().getFont("main"));
    enterText.setString("Press Enter to Continue");
    enterText.setCharacterSize(50);
    enterText.setStyle(sf::Text::Bold);
    enterText.setFillColor(sf::Color::White);

    float pulse = (std::sin(_animationClock.getElapsedTime().asSeconds() * 3.0f) + 1.0f) / 2.0f;
    sf::Color textColor = sf::Color::White;
    textColor.a = static_cast<sf::Uint8>(128 + 127 * pulse); 
    enterText.setFillColor(textColor);

    sf::FloatRect textBounds = enterText.getLocalBounds();
    enterText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    enterText.setPosition(
        _game->windowSize().x / 2.0f,
        _game->windowSize().y - 70.0f
    );

    _game->window().draw(enterText);
}


void Scene_Title::doAction(const Command& command) {
    if (command.getName() == "START" && command.getType() == "START") {
        _game->changeScene("MENU", std::make_shared<Scene_Menu>(_game));
    }
    else if (command.getName() == "EXIT" && command.getType() == "START") {
        _game->quit();
    }
}
