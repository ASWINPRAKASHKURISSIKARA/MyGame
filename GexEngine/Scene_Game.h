#pragma once
#include "Scene.h"
#include "GameEngine.h"
#include "EntityManager.h"
#include "Entity.h"
#include "BackgroundScene.h"
#include <SFML/Audio.hpp>
#include <vector>

struct Car {
    sf::Sprite sprite;
    bool goingDown = true;
};

class Scene_Game : public Scene {
private:
    GameEngine* _game;
    BackgroundScene _backgroundScene;
    EntityManager _entityManager;

    // Game objects
    sf::Texture _backgroundTexture;
    sf::Sprite _backgroundSprite1;
    sf::Sprite _backgroundSprite2;
    sf::Texture _roadTexture;
    sf::Sprite _roadSprite1;
    sf::Sprite _roadSprite2;
    sf::Texture _dogTexture;
    sf::Sprite _dogSprite;
    sf::Vector2f _dogPosition;
    sf::Texture _carSheetTexture;
    std::vector<sf::IntRect> _carFrames;
    std::vector<Car> _cars;
    sf::Texture _boneTexture;
    std::vector<sf::Sprite> _bones;
    sf::Texture _homeTexture;
    sf::Sprite _homeSprite;
    sf::Texture _gameOverTexture;
    sf::Sprite _gameOverSprite;
    sf::Texture _winTexture;
    sf::Sprite _winSprite;

    float _leftBoundary;

    // Cookie-related variables
    sf::Texture _cookieTexture;
    std::vector<sf::Sprite> _cookies;
    int _cookieCount = 0;
    sf::Clock _cookieSpawnClock;
    float _cookieSpawnInterval;
    int _requiredCookies;


    // UI elements
    sf::Text _statisticsText;
    sf::Text _distanceText;

    // Game parameters
    float _dogSpeed;
    float _carSpeed;
    float _backgroundScrollSpeed;
    float _spawnInterval;
    float _boneSpawnInterval;
    int _requiredBones;
    float WIN_DISTANCE;

    // Game state
    bool _isGameOver;
    bool _isWin;
    bool _canReachHome;
    bool _isPaused;
    float _dogDistance;
    int _boneCount;
    int _dogAnimationFrame;

    // Clocks
    sf::Clock _carSpawnClock;
    sf::Clock _boneSpawnClock;
    sf::Clock _animationClock;
    sf::Time _statisticsUpdateTime;
    unsigned int _statisticsNumFrames;

    // Hit animation
    bool _isHitAnimation = false;
    float _hitAnimationTime = 0.0f;
    float _hitAnimationDuration = 2.0f;
    sf::Vector2f _hitVelocity;
    float _hitRotation = 0.0f;
    float _gameTimeScale = 1.0f;
    float _screenShake = 0.0f;


    // Victory animation
    bool _isVictoryAnimation = false;
    float _victoryAnimationTime = 0.0f;
    float _victoryAnimationDuration = 3.0f; 
    std::vector<sf::CircleShape> _confettiParticles;
    std::vector<sf::Vector2f> _confettiVelocities;
    std::vector<sf::Color> _confettiColors;
    float _homeGlowValue = 0.0f;
    sf::CircleShape _homeGlow;

    // Health system
    int _dogHealth = 3;
    float _invincibilityTime = 0.0f;
    float _invincibilityDuration = 2.0f;
    std::vector<sf::Sprite> _healthIcons;
    sf::Texture _heartTexture;

    // Visual effects
    sf::RectangleShape _flashOverlay;
    std::vector<sf::CircleShape> _impactParticles;
    std::vector<sf::Vector2f> _particleVelocities;
    float _particleGravity = 200.0f;


    // Systems
    void sMovement(sf::Time dt);
    void sEntityMovement(sf::Time dt);
    void sScrollBackground(sf::Time dt);
    void sCollision();
    void sUserInput(const sf::Event& event);
    void sSpawnObjects(sf::Time dt);
    void spawnBone();
    void spawnCookie();
    void spawnCar();
    void sObjectMovement(sf::Time dt);
    void sCollectibles();
    void sUpdateProgress();
    void initActionMap();
    void initTextures();
    void initUI();
    void initGameParameters();
    void initSprites();
    void initCarFrames();
    void initHomeAndGameStates();
    void initGameState();
    void initClocks();

    // Helper methods
    void resetGame();
    void updateStatistics(sf::Time dt);
    void keepInBounds(Entity& e);
    sf::FloatRect getViewBounds();
    void adjustPlayerPosition();

    bool areSpritesTooClose(const sf::Sprite& sprite1, const sf::Sprite& sprite2, float minDistance = 50.0f) {
        sf::Vector2f pos1 = sprite1.getPosition();
        sf::Vector2f pos2 = sprite2.getPosition();
        float distance = std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
        return distance < minDistance;
    }


    // Methods for hit animation
    void initSounds();
    void initHealthSystem();
    void initVisualEffects();
    void startHitAnimation(const Car& car);
    void updateHitAnimation(sf::Time dt);
    void startVictoryAnimation();
    void updateVictoryAnimation(sf::Time dt);

public:
    Scene_Game(GameEngine* game);
    ~Scene_Game();

    // Scene interface implementation
    void update(sf::Time dt) override;
    void sRender() override;
    void doAction(const Command& command) override;

    // Event handlers
    void handlePlayerMovement(const sf::Vector2f& direction);
    void handleRestart();
    void handleExit();
};
