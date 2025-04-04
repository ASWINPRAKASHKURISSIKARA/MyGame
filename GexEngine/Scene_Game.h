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

    // UI elements
    sf::Text _statisticsText;
    sf::Text _distanceText;
    sf::RectangleShape _progressBarBackground;
    sf::RectangleShape _progressBarFill;

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

    // Music
    sf::Music _backgroundMusic;
    sf::Music _gameOverMusic;

    // Hit animation
    bool _isHitAnimation = false;
    float _hitAnimationTime = 0.0f;
    float _hitAnimationDuration = 2.0f;
    sf::Vector2f _hitVelocity;
    float _hitRotation = 0.0f;
    float _gameTimeScale = 1.0f;
    float _screenShake = 0.0f;
    sf::Sound _hitSound;
    sf::SoundBuffer _hitSoundBuffer;

    // Victory animation
    bool _isVictoryAnimation = false;
    float _victoryAnimationTime = 0.0f;
    float _victoryAnimationDuration = 3.0f; // 3 seconds of celebration before win screen
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
    void sScrollBackground(sf::Time dt);
    void sCollision();
    void sUserInput(const sf::Event& event);

    // Helper methods
    void resetGame();
    void updateStatistics(sf::Time dt);
    void keepInBounds(Entity& e);
    sf::FloatRect getViewBounds();
    void adjustPlayerPosition();

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
