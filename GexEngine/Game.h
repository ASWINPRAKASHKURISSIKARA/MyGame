//
// Created by David Burchill on 2023-09-27.
//

#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"
#include "Scene.h"


struct PlayerConfig {
    float                       radius{30};
    float                       speed{200};
    sf::Color                   fillColor{255,255,0};
    sf::Color                   outlineColor{255,255,255};
    float                       outlineThickness{3};
    int                         nVerticies{3};
};

enum class GameState {
    Title,
    Menu,
    Background
};



class Game {
private:
    const static sf::Time TIME_PER_FRAME;

    sf::Vector2u                windowSize{1280,768};
    sf::RenderWindow            window;
    EntityManager               entityManager;
    sf::Font                    font;
    sPtrEntt                    player{nullptr};

    PlayerConfig                playerConfig;
    bool                        isRunning{true};
    bool                        isPaused{false};

    GameState currentState = GameState::Title;

    sf::Texture titleTexture;
    sf::Sprite titleSprite;
    sf::Texture menuTexture;
    sf::Sprite menuSprite;

    const std::string assetsPath = "../assets/";

    int currentMenuOption = 0;  
    const int menuOptionCount = 3;

    // stats
    sf::Text                    statisticsText;
    sf::Time                    statisticsUpdateTime{sf::Time::Zero};
    unsigned int                statisticsNumFrames{0};

    Scene scene;

    // systems
    void                        sMovement(sf::Time dt);
    void                        sCollision();
    void                        sRender();
    void                        sUpdate(sf::Time dt);
    void                        sUserInput();


    // helper functions
    void                        keepInBounds(Entity &e);
    void                        adjustPlayerPosition();
    void                        init(const std::string &path);
    void                        loadConfigFromFile(const std::string &path);
    void                        updateStatistics(sf::Time dt);
    sf::FloatRect               getViewBounds();

public:

    Game(const std::string &path);
    void run();

};


#endif //BREAKOUT_GAME_H
