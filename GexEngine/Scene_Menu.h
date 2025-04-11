#pragma once
#include "Scene.h"
#include "GameEngine.h"

enum class MenuOption {
    START_GAME,
    INSTRUCTIONS,
    CONTROLS,
    STORY,
    COUNT
};

enum class MenuState {
    MAIN_MENU,
    INSTRUCTIONS,
    CONTROLS,
    STORY
};

class Scene_Menu : public Scene {
private:
    GameEngine* _game;
    sf::Texture _menuTexture;
    sf::Sprite _menuSprite;

    MenuOption _currentOption = MenuOption::START_GAME;
    MenuState _menuState = MenuState::MAIN_MENU;

    sf::Text _titleText;
    std::vector<sf::Text> _menuOptions;
    sf::Text _footerText;

    sf::RectangleShape _highlightRect;

    // Content for different menu states
    sf::Text _instructionsText;
    sf::Text _controlsText;
    sf::Text _storyText;
    sf::Text _backText;

    void initMenuTexts();
    void initContentTexts();

public:
    Scene_Menu(GameEngine* game);
    void update(sf::Time dt) override;
    void sRender() override;
    void doAction(const Command& command) override;

    void renderMainMenu();
    void renderInstructions();
    void renderControls();
    void renderStory();
};
