#include "Scene_Menu.h"
#include "Scene_Game.h"
#include "Assets.h"
#include <iostream>

Scene_Menu::Scene_Menu(GameEngine* game) : _game(game) {
    _actionMap[sf::Keyboard::Up] = "UP";
    _actionMap[sf::Keyboard::Down] = "DOWN";
    _actionMap[sf::Keyboard::Enter] = "SELECT";
    _actionMap[sf::Keyboard::Escape] = "BACK";

    _menuTexture = Assets::getInstance().getTexture("menu");
    _menuSprite.setTexture(_menuTexture);
    _menuSprite.setScale(
        _game->windowSize().x / _menuTexture.getSize().x,
        _game->windowSize().y / _menuTexture.getSize().y
    );

    _highlightRect.setSize(sf::Vector2f(400, 70));
    _highlightRect.setFillColor(sf::Color(255, 255, 255, 100));

    initMenuTexts();
    initContentTexts();
}

void Scene_Menu::initMenuTexts() {
    auto& font = Assets::getInstance().getFont("main");

    // Title
    _titleText.setFont(font);
    _titleText.setString("PAWSTACLE DASH");
    _titleText.setCharacterSize(60);
    _titleText.setStyle(sf::Text::Bold);
    _titleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = _titleText.getLocalBounds();
    _titleText.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
    _titleText.setPosition(_game->windowSize().x / 2.0f, 100.0f);

    // Menu options
    std::vector<std::string> optionTexts = {
        "Start Game",
        "Instructions",
        "Controls",
        "Story"
    };

    float startY = 250.0f;
    float spacing = 80.0f;

    for (size_t i = 0; i < optionTexts.size(); ++i) {
        sf::Text optionText;
        optionText.setFont(font);
        optionText.setString(optionTexts[i]);
        optionText.setCharacterSize(40);
        optionText.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = optionText.getLocalBounds();
        optionText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
        optionText.setPosition(_game->windowSize().x / 2.0f, startY + i * spacing);

        _menuOptions.push_back(optionText);
    }

    // Footer
    _footerText.setFont(font);
    _footerText.setString("Use Arrow Keys to Navigate | Press Enter to Select");
    _footerText.setCharacterSize(20);
    _footerText.setFillColor(sf::Color::White);
    sf::FloatRect footerBounds = _footerText.getLocalBounds();
    _footerText.setOrigin(footerBounds.width / 2.0f, footerBounds.height / 2.0f);
    _footerText.setPosition(_game->windowSize().x / 2.0f, _game->windowSize().y - 50.0f);

    // Back text for sub-menus
    _backText.setFont(font);
    _backText.setString("Press ESC to go back to main menu");
    _backText.setCharacterSize(20);
    _backText.setFillColor(sf::Color::White);
    sf::FloatRect backBounds = _backText.getLocalBounds();
    _backText.setOrigin(backBounds.width / 2.0f, backBounds.height / 2.0f);
    _backText.setPosition(_game->windowSize().x / 2.0f, _game->windowSize().y - 80.0f);
}

void Scene_Menu::initContentTexts() {
    auto& font = Assets::getInstance().getFont("main");

    // Instructions
    _instructionsText.setFont(font);
    _instructionsText.setString(
        "INSTRUCTIONS\n\n"
        "Goal:\n"
        "- Guide your dog safely home\n"
        "- Collect at least " + std::to_string(Assets::getInstance().getInt("RequiredBones", 10)) + " bones\n"
        "- Collect at least " + std::to_string(Assets::getInstance().getInt("RequiredCookies", 10)) + " cookies\n"
        "- Travel the required distance(3000m) to make home appear\n\n"

        "Lives:\n"
        "- You start with " + std::to_string(Assets::getInstance().getInt("HitAnimation.DogHealth", 3)) + " lives (hearts)\n"
        "- Colliding with cars costs one life\n"
        "- Colliding with cars causes your dog to bounce and spin\n\n"

        "Winning:\n"
        "- Reach the required distance(3000m)\n"
        "- Collect enough bones and cookies\n"
        "- Find and reach your home\n\n"

        "Losing:\n"
        "- Lose all your lives by hitting cars"
    );
    _instructionsText.setCharacterSize(30);
    _instructionsText.setFillColor(sf::Color::White);
    sf::FloatRect instrBounds = _instructionsText.getLocalBounds();
    _instructionsText.setOrigin(instrBounds.width / 2.0f, instrBounds.height / 2.0f);
    _instructionsText.setPosition(_game->windowSize().x / 2.0f, _game->windowSize().y / 2.0f);

    // Controls
    _controlsText.setFont(font);
    _controlsText.setString(
        "CONTROLS\n\n"
        "W or Up Arrow - Move Up\n"
        "S or Down Arrow - Move Down\n"
        "A or Left Arrow - Move Left\n"
        "D or Right Arrow - Move Right\n"
        "R - Restart Game\n"
        "ESC - Exit to Menu"
    );
    _controlsText.setCharacterSize(30);
    _controlsText.setFillColor(sf::Color::White);
    sf::FloatRect ctrlBounds = _controlsText.getLocalBounds();
    _controlsText.setOrigin(ctrlBounds.width / 2.0f, ctrlBounds.height / 2.0f);
    _controlsText.setPosition(_game->windowSize().x / 2.0f, _game->windowSize().y / 2.0f);

    // Story
    _storyText.setFont(font);
    _storyText.setString(
        "STORY\n\n"
        "After a day of adventure, your loyal dog has wandered too far\n"
        "from home. In this Pawstacle Dash, help guide your furry friend back\n"
        "through busy streets, collecting treats along the way. Avoid the traffic\n"
        "and find the path that leads back to the warmth and safety of home.\n\n"
        "Can you help your dog complete this dash home?"
    );
    _storyText.setCharacterSize(30);
    _storyText.setFillColor(sf::Color::White);
    sf::FloatRect storyBounds = _storyText.getLocalBounds();
    _storyText.setOrigin(storyBounds.width / 2.0f, storyBounds.height / 2.0f);
    _storyText.setPosition(_game->windowSize().x / 2.0f, _game->windowSize().y / 2.0f);
}

void Scene_Menu::update(sf::Time dt) {
    // Any animations or updates can go here
}

void Scene_Menu::sRender() {
    _game->window().draw(_menuSprite);

    switch (_menuState) {
    case MenuState::MAIN_MENU:
        renderMainMenu();
        break;
    case MenuState::INSTRUCTIONS:
        renderInstructions();
        break;
    case MenuState::CONTROLS:
        renderControls();
        break;
    case MenuState::STORY:
        renderStory();
        break;
    }
}

void Scene_Menu::renderMainMenu() {
    _game->window().draw(_titleText);

    // Calculate highlight position
    int optionIndex = static_cast<int>(_currentOption);
    _highlightRect.setPosition(
        _game->windowSize().x / 2.0f - _highlightRect.getSize().x / 2.0f,
        _menuOptions[optionIndex].getPosition().y - _highlightRect.getSize().y / 2.0f
    );

    _game->window().draw(_highlightRect);

    for (auto& option : _menuOptions) {
        _game->window().draw(option);
    }

    _game->window().draw(_footerText);
}

void Scene_Menu::renderInstructions() {
    _game->window().draw(_instructionsText);
    _game->window().draw(_backText);
}

void Scene_Menu::renderControls() {
    _game->window().draw(_controlsText);
    _game->window().draw(_backText);
}

void Scene_Menu::renderStory() {
    _game->window().draw(_storyText);
    _game->window().draw(_backText);
}

void Scene_Menu::doAction(const Command& command) {
    if (_menuState == MenuState::MAIN_MENU) {
        if (command.getName() == "UP" && command.getType() == "START") {
            int currentIndex = static_cast<int>(_currentOption);
            currentIndex = (currentIndex - 1 + static_cast<int>(MenuOption::COUNT)) % static_cast<int>(MenuOption::COUNT);
            _currentOption = static_cast<MenuOption>(currentIndex);
        }
        else if (command.getName() == "DOWN" && command.getType() == "START") {
            int currentIndex = static_cast<int>(_currentOption);
            currentIndex = (currentIndex + 1) % static_cast<int>(MenuOption::COUNT);
            _currentOption = static_cast<MenuOption>(currentIndex);
        }
        else if (command.getName() == "SELECT" && command.getType() == "START") {
            switch (_currentOption) {
            case MenuOption::START_GAME:
                _game->changeScene("GAME", std::make_shared<Scene_Game>(_game));
                break;
            case MenuOption::INSTRUCTIONS:
                _menuState = MenuState::INSTRUCTIONS;
                break;
            case MenuOption::CONTROLS:
                _menuState = MenuState::CONTROLS;
                break;
            case MenuOption::STORY:
                _menuState = MenuState::STORY;
                break;
            default:
                break;
            }
        }
    }
    else {
        // In a sub-menu, ESC returns to main menu
        if (command.getName() == "BACK" && command.getType() == "START") {
            _menuState = MenuState::MAIN_MENU;
        }
    }
}
