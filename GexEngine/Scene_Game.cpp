#include "Scene_Game.h"
#include "Command.h" 
#include "Entity.h"
#include "Assets.h"
#include "Utilities.h"
#include "Scene_Menu.h"
#include "Scene_Title.h"
#include "GameEngine.h"
#include <iostream>
#include <cmath>

struct CTransform;
struct CCollision;

Scene_Game::Scene_Game(GameEngine* game)
    : _game(game), _backgroundScene("background.png") {

    _actionMap[sf::Keyboard::W] = "MOVE_UP";
    _actionMap[sf::Keyboard::S] = "MOVE_DOWN";
    _actionMap[sf::Keyboard::A] = "MOVE_LEFT";
    _actionMap[sf::Keyboard::D] = "MOVE_RIGHT";
    _actionMap[sf::Keyboard::R] = "RESTART";
    _actionMap[sf::Keyboard::Escape] = "EXIT";

    auto& assets = Assets::getInstance();

    _statisticsText.setFont(assets.getFont("main"));
    _statisticsText.setPosition(assets.getVector("StatisticsPosition", sf::Vector2f(15.0f, 15.0f)));
    _statisticsText.setCharacterSize(assets.getInt("StatisticsSize", 15));

    _distanceText.setFont(assets.getFont("main"));
    _distanceText.setCharacterSize(assets.getInt("DistanceTextSize", 20));
    _distanceText.setFillColor(sf::Color::White);
    _distanceText.setPosition(assets.getVector("DistanceTextPosition", sf::Vector2f(15.f, 40.f)));

    _backgroundTexture = assets.getTexture("background");
    _roadTexture = assets.getTexture("road");
    _dogTexture = assets.getTexture("dog");
    _carSheetTexture = assets.getTexture("cars");
    _boneTexture = assets.getTexture("bone");
    _homeTexture = assets.getTexture("home");
    _gameOverTexture = assets.getTexture("gameover");
    _winTexture = assets.getTexture("winner");

    _dogSpeed = assets.getFloat("DogSpeed", 25.0f);
    _carSpeed = assets.getFloat("CarSpeed", 100.0f);
    _backgroundScrollSpeed = assets.getFloat("BackgroundScrollSpeed", 200.0f);
    _spawnInterval = assets.getFloat("CarSpawnInterval", 1.5f);
    _boneSpawnInterval = assets.getFloat("BoneSpawnInterval", 3.0f);
    _requiredBones = assets.getInt("RequiredBones", 10);
    WIN_DISTANCE = assets.getFloat("WinDistance", 1000.0f);

    _hitAnimationDuration = assets.getFloat("HitAnimation.Duration", 2.0f);
    _invincibilityDuration = assets.getFloat("HitAnimation.InvincibilityDuration", 2.0f);
    _dogHealth = assets.getInt("HitAnimation.DogHealth", 3);
    _particleGravity = assets.getFloat("HitAnimation.ParticleGravity", 200.0f);

    initSounds();
    initHealthSystem();
    initVisualEffects();

    _backgroundSprite1.setTexture(_backgroundTexture);
    _backgroundSprite2.setTexture(_backgroundTexture);
    _backgroundSprite1.setPosition(0, 0);
    _backgroundSprite2.setPosition(0, -static_cast<float>(_backgroundTexture.getSize().y));

    _roadSprite1.setTexture(_roadTexture);
    _roadSprite2.setTexture(_roadTexture);
    sf::Vector2f roadPos = assets.getVector("RoadPosition", sf::Vector2f(470.f, 0.f));
    _roadSprite1.setPosition(roadPos);
    _roadSprite2.setPosition(roadPos.x, -static_cast<float>(_roadTexture.getSize().y));

    _dogTexture.setSmooth(true);
    _dogSprite.setTexture(_dogTexture);
    _dogSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    _dogPosition = assets.getVector("DogStartPosition", sf::Vector2f(640.f, 384.f));
    _dogSprite.setPosition(_dogPosition);
    _dogSprite.setScale(assets.getFloat("DogScale", 2.0f), assets.getFloat("DogScale", 2.0f));

    int carWidth = 120;
    int carHeight = 220;
    int numCars = 3;

    for (int i = 0; i < numCars; i++) {
        int x = i * carWidth;
        int y = 0;
        _carFrames.push_back(sf::IntRect(x, y, carWidth, carHeight));
    }

    _homeSprite.setTexture(_homeTexture);
    _homeSprite.setScale(0.2f, 0.2f);
    sf::FloatRect bounds = _homeSprite.getLocalBounds();
    _homeSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    _homeSprite.setPosition(
        static_cast<float>(_game->windowSize().x) / 1.2f,
        80.f
    );

    _gameOverSprite.setTexture(_gameOverTexture);
    _gameOverSprite.setScale(
        static_cast<float>(_game->windowSize().x) / _gameOverTexture.getSize().x,
        static_cast<float>(_game->windowSize().y) / _gameOverTexture.getSize().y
    );

    _winSprite.setTexture(_winTexture);
    _winSprite.setScale(
        static_cast<float>(_game->windowSize().x) / _winTexture.getSize().x,
        static_cast<float>(_game->windowSize().y) / _winTexture.getSize().y
    );

    sf::Vector2f progressBarPos = assets.getVector("ProgressBarPosition", sf::Vector2f(490.f, 20.f));
    sf::Vector2f progressBarSize = assets.getVector("ProgressBarSize", sf::Vector2f(300.f, 20.f));

    _progressBarBackground.setSize(progressBarSize);
    _progressBarBackground.setFillColor(sf::Color(50, 50, 50, 200));
    _progressBarBackground.setPosition(progressBarPos);

    _progressBarFill.setSize(sf::Vector2f(0, progressBarSize.y));
    _progressBarFill.setFillColor(sf::Color(0, 255, 0, 200));
    _progressBarFill.setPosition(progressBarPos);

    if (!_backgroundMusic.openFromFile(assets.getString("Sound.background", "../assets/backmusic.mp3"))) {
        std::cerr << "Failed to load background music!" << std::endl;
    }
    else {
        _backgroundMusic.setLoop(true);
        _backgroundMusic.setVolume(100);
        _backgroundMusic.play();
    }

    if (!_gameOverMusic.openFromFile(assets.getString("Sound.gameover", "../assets/gameover.mp3"))) {
        std::cerr << "Failed to load game over music!" << std::endl;
    }

    _carSpawnClock.restart();
    _boneSpawnClock.restart();
    _animationClock.restart();

    _isGameOver = false;
    _isWin = false;
    _canReachHome = false;
    _isPaused = false;
    _dogDistance = 0.0f;
    _boneCount = 0;
    _dogAnimationFrame = 0;

    std::cout << "Scene_Game initialized successfully" << std::endl;
}


Scene_Game::~Scene_Game() {
    _backgroundMusic.stop();
    _gameOverMusic.stop();
}

void Scene_Game::update(sf::Time dt) {
    sf::Time scaledDt = dt * _gameTimeScale;

    if (_invincibilityTime > 0.0f) {
        _invincibilityTime -= dt.asSeconds();

        int flashFrequency = Assets::getInstance().getInt("InvincibilityFlashFrequency", 8);
        bool visible = static_cast<int>(_invincibilityTime * flashFrequency) % 2 == 0;
        _dogSprite.setColor(visible ? sf::Color::White : sf::Color(255, 255, 255, 128));
    }
    else {
        _dogSprite.setColor(sf::Color::White);
    }

    if (_isPaused) return;

    _entityManager.update();

    if (_isGameOver || _isWin) return;

    if (_isHitAnimation) {
        updateHitAnimation(dt); 
        return; 
    }

    sf::Vector2f direction(0.f, 0.f);
    bool isMoving = false;
    bool isMovingUp = false;  

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y -= 1;
        _dogSprite.setTextureRect(sf::IntRect(0, 96, 32, 32));  
        isMoving = true;
        isMovingUp = true;  
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y += 1;
        _dogSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));  
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x -= 1;
        _dogSprite.setTextureRect(sf::IntRect(0, 32, 32, 32));  
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x += 1;
        _dogSprite.setTextureRect(sf::IntRect(0, 64, 32, 32));  
        isMoving = true;
    }

    if (isMoving) {
        if (direction.x != 0 && direction.y != 0) {
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= length;
        }

        sf::Vector2f oldPosition = _dogPosition;

        _dogPosition += direction * _dogSpeed * scaledDt.asSeconds();  

        sf::Vector2u windowSize = _game->window().getSize();
        _dogPosition.x = std::max(0.f, std::min(_dogPosition.x, windowSize.x - 32.f));
        _dogPosition.y = std::max(0.f, std::min(_dogPosition.y, windowSize.y - 32.f));

        _dogSprite.setPosition(_dogPosition);

        float distanceMoved = std::sqrt(
            std::pow(_dogPosition.x - oldPosition.x, 2) +
            std::pow(_dogPosition.y - oldPosition.y, 2)
        );

        if (isMovingUp) {
            _dogDistance += distanceMoved;
            sScrollBackground(scaledDt);  
        }

        if (distanceMoved > 0.1f && _animationClock.getElapsedTime().asSeconds() > 0.1f) {
            _dogAnimationFrame = (_dogAnimationFrame + 1) % 3;
            sf::IntRect textureRect = _dogSprite.getTextureRect();
            int row = textureRect.top / 32; 
            textureRect.left = _dogAnimationFrame * 32;
            _dogSprite.setTextureRect(textureRect);
            _animationClock.restart();
        }
    }

    sCollision();

    for (auto& bone : _bones) {
        bone.move(0, 100.f * scaledDt.asSeconds());  
    }

    _bones.erase(std::remove_if(_bones.begin(), _bones.end(), [&](sf::Sprite& b) {
        return b.getPosition().y > _game->window().getSize().y;
        }), _bones.end());

    for (auto it = _bones.begin(); it != _bones.end();) {
        if (_dogSprite.getGlobalBounds().intersects(it->getGlobalBounds())) {
            _boneCount++;
            it = _bones.erase(it);
        }
        else {
            ++it;
        }
    }

    if (_boneSpawnClock.getElapsedTime().asSeconds() > _boneSpawnInterval) {
        sf::Sprite bone;
        bone.setTexture(_boneTexture);
        bone.setScale(0.1f, 0.1f);
        int laneX[] = { 450, 640, 830 };
        bone.setPosition(static_cast<float>(laneX[rand() % 3]), -100.f);
        _bones.push_back(bone);
        _boneSpawnClock.restart();
    }

    if (_carSpawnClock.getElapsedTime().asSeconds() >= _spawnInterval) {
        Car car;
        car.sprite.setTexture(_carSheetTexture);
        int carIndex = rand() % 3;
        car.sprite.setTextureRect(_carFrames[carIndex]);
        int laneX[] = { 450, 640, 830 };
        int laneIndex = rand() % 3;
        float startY;
        if (laneIndex == 2) {
            startY = static_cast<float>(_game->window().getSize().y + 220); 
            car.goingDown = false;
            car.sprite.setScale(0.5f, -0.5f);
            car.sprite.setOrigin(0, car.sprite.getLocalBounds().height);
        }
        else {
            startY = -220.f; 
            car.goingDown = true;
            car.sprite.setScale(0.5f, 0.5f);
        }
        car.sprite.setPosition(static_cast<float>(laneX[laneIndex]), startY);
        _cars.push_back(car);
        _carSpawnClock.restart();
    }

    if (_dogDistance >= WIN_DISTANCE && _boneCount >= _requiredBones) {
        _canReachHome = true;  
    }

    if (_canReachHome && _dogSprite.getGlobalBounds().intersects(_homeSprite.getGlobalBounds()) && !_isVictoryAnimation) {
        startVictoryAnimation(); 
    }

    if (_isVictoryAnimation) {
        updateVictoryAnimation(dt);
    }

    for (auto& car : _cars) {
        float dy = (car.goingDown ? 1.f : -1.f) * _carSpeed * scaledDt.asSeconds(); 
        car.sprite.move(0, dy);
    }

    _cars.erase(std::remove_if(_cars.begin(), _cars.end(), [&](const Car& c) {
        float y = c.sprite.getPosition().y;
        return (c.goingDown && y > _game->window().getSize().y) || (!c.goingDown && y < -220.f);
        }), _cars.end());

    if (_invincibilityTime <= 0.0f) {  
        for (const auto& car : _cars) {
            sf::FloatRect dogBounds = _dogSprite.getGlobalBounds();
            sf::FloatRect carBounds = car.sprite.getGlobalBounds();

            dogBounds.left += 10;
            dogBounds.width -= 20;
            dogBounds.top += 5;
            dogBounds.height -= 10;

            if (dogBounds.intersects(carBounds)) {
                _dogHealth--;

                if (_dogHealth >= 0 && _dogHealth < _healthIcons.size()) {
                    _healthIcons[_dogHealth].setColor(sf::Color(100, 100, 100, 128)); 
                }

                startHitAnimation(car);

                _invincibilityTime = _invincibilityDuration;

                break;
            }
        }
    }

    float progress = _dogDistance / WIN_DISTANCE;
    progress = std::min(progress, 1.0f);
    _progressBarFill.setSize(sf::Vector2f(300 * progress, 20));

    updateStatistics(dt); 
}


void Scene_Game::sRender() {
    sf::View originalView = _game->window().getView();
    sf::View view = originalView;

    if (_screenShake > 0.0f) {
        float shakeX = (rand() % 100 - 50) * 0.01f * _screenShake;
        float shakeY = (rand() % 100 - 50) * 0.01f * _screenShake;
        view.setCenter(view.getCenter() + sf::Vector2f(shakeX, shakeY));
        _game->window().setView(view);
    }

    _game->window().draw(_backgroundSprite1);
    _game->window().draw(_backgroundSprite2);
    _game->window().draw(_roadSprite1);
    _game->window().draw(_roadSprite2);

    for (const auto& car : _cars)
        _game->window().draw(car.sprite);

    for (const auto& bone : _bones)
        _game->window().draw(bone);

    _game->window().draw(_dogSprite);

    if (_canReachHome && !_isWin) {
        if (_isVictoryAnimation) {
            _game->window().draw(_homeGlow);
        }
        _game->window().draw(_homeSprite);
    }

    if (_isVictoryAnimation) {
        for (const auto& confetti : _confettiParticles) {
            _game->window().draw(confetti);
        }
    }


    for (const auto& particle : _impactParticles) {
        _game->window().draw(particle);
    }

    _game->window().draw(_flashOverlay);

    for (const auto& heart : _healthIcons) {
        _game->window().draw(heart);
    }

    if (_isGameOver) {
        _game->window().draw(_gameOverSprite);
    }

    if (_isWin) {
        _game->window().draw(_winSprite);
    }

    _distanceText.setString("Distance: " + std::to_string(static_cast<int>(_dogDistance)) + " m\nBones: " + std::to_string(_boneCount));
    _game->window().draw(_distanceText);

    _game->window().draw(_progressBarBackground);
    _game->window().draw(_progressBarFill);

    if (_screenShake > 0.0f) {
        _game->window().setView(originalView);
    }
}


void Scene_Game::doAction(const Command& command) {
    if (_isGameOver || _isPaused) return;

    if (command.getName() == "MOVE_UP") {
        sf::Vector2f oldPosition = _dogPosition;
        _dogPosition.y -= _dogSpeed * command.getDeltaTime().asSeconds();

        _dogSprite.setTextureRect(sf::IntRect(0, 96, 32, 32));

        float distanceMoved = std::abs(_dogPosition.y - oldPosition.y);
        _dogDistance += distanceMoved;
    }
    else if (command.getName() == "MOVE_DOWN") {
        sf::Vector2f oldPosition = _dogPosition;
        _dogPosition.y += _dogSpeed * command.getDeltaTime().asSeconds();

        _dogSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

        float distanceMoved = std::abs(_dogPosition.y - oldPosition.y);
        _dogDistance += distanceMoved;
    }
    else if (command.getName() == "MOVE_LEFT") {
        sf::Vector2f oldPosition = _dogPosition;
        _dogPosition.x -= _dogSpeed * command.getDeltaTime().asSeconds();

        _dogSprite.setTextureRect(sf::IntRect(0, 32, 32, 32));

        float distanceMoved = std::abs(_dogPosition.x - oldPosition.x);
        _dogDistance += distanceMoved;
    }
    else if (command.getName() == "MOVE_RIGHT") {
        sf::Vector2f oldPosition = _dogPosition;
        _dogPosition.x += _dogSpeed * command.getDeltaTime().asSeconds();

        _dogSprite.setTextureRect(sf::IntRect(0, 64, 32, 32));

        float distanceMoved = std::abs(_dogPosition.x - oldPosition.x);
        _dogDistance += distanceMoved;
    }
    else if (command.getName() == "RESTART" && _isGameOver) {
        resetGame();
    }
    else if (command.getName() == "EXIT") {
        _game->quit();
    }

    sf::Vector2u windowSize = _game->window().getSize();
    _dogPosition.x = std::max(0.f, std::min(_dogPosition.x, windowSize.x - 32.f));
    _dogPosition.y = std::max(0.f, std::min(_dogPosition.y, windowSize.y - 32.f));

    _dogSprite.setPosition(_dogPosition);
}




void Scene_Game::sMovement(sf::Time dt) {
    if (_isWin) return; 

    for (auto& e : _entityManager.getEntities()) {
        if (e->hasComponent<CTransform>()) {
            auto& tfm = e->getComponent<CTransform>();
            tfm.pos += tfm.vel * dt.asSeconds();
            keepInBounds(*e);
        }
    }
}


void Scene_Game::sScrollBackground(sf::Time dt) {
    float scrollAmount = _backgroundScrollSpeed * dt.asSeconds(); 

    _backgroundSprite1.move(0, scrollAmount);
    _backgroundSprite2.move(0, scrollAmount);

    if (_backgroundSprite1.getPosition().y >= static_cast<float>(_backgroundTexture.getSize().y)) {
        _backgroundSprite1.setPosition(0, _backgroundSprite2.getPosition().y - static_cast<float>(_backgroundTexture.getSize().y));
    }

    if (_backgroundSprite2.getPosition().y >= static_cast<float>(_backgroundTexture.getSize().y)) {
        _backgroundSprite2.setPosition(0, _backgroundSprite1.getPosition().y - static_cast<float>(_backgroundTexture.getSize().y));
    }

    _roadSprite1.move(0, scrollAmount);
    _roadSprite2.move(0, scrollAmount);

    if (_roadSprite1.getPosition().y >= static_cast<float>(_roadTexture.getSize().y)) {
        _roadSprite1.setPosition(_roadSprite1.getPosition().x, _roadSprite2.getPosition().y - _roadTexture.getSize().y);
    }

    if (_roadSprite2.getPosition().y >= static_cast<float>(_roadTexture.getSize().y)) {
        _roadSprite2.setPosition(_roadSprite2.getPosition().x, _roadSprite1.getPosition().y - _roadTexture.getSize().y);
    }
}


void Scene_Game::sCollision() {
    if (_invincibilityTime > 0.0f || _isHitAnimation) return;

    for (const auto& car : _cars) {
        sf::FloatRect dogBounds = _dogSprite.getGlobalBounds();
        sf::FloatRect carBounds = car.sprite.getGlobalBounds();

        dogBounds.left += 10;
        dogBounds.width -= 20;
        dogBounds.top += 5;
        dogBounds.height -= 10;

        if (dogBounds.intersects(carBounds)) {
            _dogHealth--;

            if (_healthIcons.size() > 0) {
                _healthIcons.pop_back();
            }

            startHitAnimation(car);

            _invincibilityTime = _invincibilityDuration;

            return;
        }
    }
}


void Scene_Game::resetGame() {
    _isGameOver = false;
    _isWin = false;
    _canReachHome = false;
    _dogDistance = 0.0f;
    _boneCount = 0;
    _cars.clear();
    _bones.clear();
    _dogPosition = sf::Vector2f(640.f, 600.f);
    _dogSprite.setPosition(_dogPosition);
    _dogSprite.setRotation(0.0f);

    _dogHealth = Assets::getInstance().getInt("DogHealth", 3);
    _invincibilityTime = 0.0f;
    _healthIcons.clear();

    for (int i = 0; i < _dogHealth; i++) {
        sf::Sprite heart;
        heart.setTexture(_heartTexture);
        float scale = Assets::getInstance().getFloat("HeartScale", 0.05f);
        heart.setScale(scale, scale);
        sf::Vector2f basePos = Assets::getInstance().getVector("HeartBasePosition", sf::Vector2f(20.f, 20.f));
        float spacing = Assets::getInstance().getFloat("HeartSpacing", 40.f);
        heart.setPosition(basePos.x + i * spacing, basePos.y);
        _healthIcons.push_back(heart);
    }

    _isHitAnimation = false;
    _hitAnimationTime = 0.0f;
    _isVictoryAnimation = false;
    _victoryAnimationTime = 0.0f;
    _confettiParticles.clear();
    _confettiVelocities.clear();
    _confettiColors.clear();
    _gameTimeScale = 1.0f;
    _screenShake = 0.0f;
    _flashOverlay.setFillColor(sf::Color(255, 0, 0, 0));
    _impactParticles.clear();
    _particleVelocities.clear();

    _backgroundMusic.play();
}


void Scene_Game::updateStatistics(sf::Time dt) {
    _statisticsUpdateTime += dt;
    _statisticsNumFrames += 1;
    if (_statisticsUpdateTime >= sf::seconds(1.0f)) {
        _statisticsText.setString("");
        _statisticsUpdateTime -= sf::seconds(1.0f);
        _statisticsNumFrames = 0;
    }
}

void Scene_Game::keepInBounds(Entity& e) {
    if (e.hasComponent<CCollision>()) {
        auto cr = e.getComponent<CCollision>().radius;
        auto& tfm = e.getComponent<CTransform>();
        auto wbounds = _game->window().getSize();

        if (tfm.pos.x < cr || tfm.pos.x >(wbounds.x - cr)) {
            tfm.vel.x *= -1;
            tfm.pos.x = (tfm.pos.x < cr) ? cr : wbounds.x - cr;
        }

        if (tfm.pos.y < cr || tfm.pos.y >(wbounds.y - cr)) {
            tfm.vel.y *= -1;
            tfm.pos.y = (tfm.pos.y < cr) ? cr : wbounds.y - cr;
        }
    }
}


sf::FloatRect Scene_Game::getViewBounds() {
    sf::View view = _game->window().getView();
    return sf::FloatRect(
        view.getCenter().x - (view.getSize().x / 2.f),
        view.getCenter().y - (view.getSize().y / 2.f),
        view.getSize().x,
        view.getSize().y
    );
}

void Scene_Game::initSounds() {
    if (!_hitSoundBuffer.loadFromFile(Assets::getInstance().getString("Sound.hit", "../assets/dog_hit.wav"))) {
        std::cerr << "Failed to load dog hit sound\n";
    }
    else {
        _hitSound.setBuffer(_hitSoundBuffer);
        _hitSound.setVolume(Assets::getInstance().getFloat("HitSoundVolume", 80.0f));
    }
}

void Scene_Game::initHealthSystem() {
    _heartTexture = Assets::getInstance().getTexture("heart");

    for (int i = 0; i < _dogHealth; i++) {
        sf::Sprite heart;
        heart.setTexture(_heartTexture);
        float scale = Assets::getInstance().getFloat("HeartScale", 0.05f);
        heart.setScale(scale, scale);
        sf::Vector2f basePos = Assets::getInstance().getVector("HeartBasePosition", sf::Vector2f(20.f, 20.f));
        float spacing = Assets::getInstance().getFloat("HeartSpacing", 40.f);
        heart.setPosition(basePos.x + i * spacing, basePos.y);
        _healthIcons.push_back(heart);
    }
}

void Scene_Game::initVisualEffects() {
    _flashOverlay.setSize(_game->windowSize());
    _flashOverlay.setFillColor(sf::Color(255, 0, 0, 0)); 

    _homeGlow.setRadius(100.0f);
    _homeGlow.setFillColor(sf::Color(255, 255, 100, 0)); 
    _homeGlow.setOrigin(_homeGlow.getRadius(), _homeGlow.getRadius());
    _homeGlow.setPosition(_homeSprite.getPosition());
}

void Scene_Game::startHitAnimation(const Car& car) {
    _isHitAnimation = true;
    _hitAnimationTime = 0.0f;

    sf::Vector2f hitDirection;
    if (car.goingDown) {
        hitDirection = sf::Vector2f(0.5f, 1.0f); 
    }
    else {
        hitDirection = sf::Vector2f(-0.5f, -1.0f); 
    }

    float length = std::sqrt(hitDirection.x * hitDirection.x + hitDirection.y * hitDirection.y);
    hitDirection /= length;
    float hitForce = Assets::getInstance().getFloat("HitForce", 300.0f);
    _hitVelocity = hitDirection * hitForce;

    _hitSound.play();

    _gameTimeScale = Assets::getInstance().getFloat("HitTimeScale", 0.5f);

    _screenShake = Assets::getInstance().getFloat("ScreenShakeIntensity", 10.0f);

    _flashOverlay.setFillColor(sf::Color(255, 0, 0,
        Assets::getInstance().getInt("FlashAlpha", 180)));

    int particleCount = Assets::getInstance().getInt("ImpactParticleCount", 20);
    for (int i = 0; i < particleCount; i++) {
        sf::CircleShape particle;
        particle.setRadius(2.0f + (rand() % 4));
        particle.setFillColor(sf::Color(255, 255, 255));
        particle.setPosition(_dogPosition);

        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 100.0f + (rand() % 200);
        sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);

        _impactParticles.push_back(particle);
        _particleVelocities.push_back(velocity);
    }
}

void Scene_Game::updateHitAnimation(sf::Time dt) {
    if (!_isHitAnimation) return;

    _hitAnimationTime += dt.asSeconds();

    _hitVelocity.y += _particleGravity * dt.asSeconds(); 
    _dogPosition += _hitVelocity * dt.asSeconds();

    float rotationSpeed = Assets::getInstance().getFloat("HitRotationSpeed", 360.0f);
    _hitRotation += rotationSpeed * dt.asSeconds();
    _dogSprite.setRotation(_hitRotation);

    _dogSprite.setPosition(_dogPosition);

    float timeScaleRecoveryRate = Assets::getInstance().getFloat("TimeScaleRecoveryRate", 0.5f);
    if (_gameTimeScale < 1.0f) {
        _gameTimeScale += dt.asSeconds() * timeScaleRecoveryRate;
        if (_gameTimeScale > 1.0f) _gameTimeScale = 1.0f;
    }

    float shakeDecayRate = Assets::getInstance().getFloat("ShakeDecayRate", 20.0f);
    if (_screenShake > 0.0f) {
        _screenShake -= dt.asSeconds() * shakeDecayRate;
        if (_screenShake < 0.0f) _screenShake = 0.0f;
    }

    sf::Color flashColor = _flashOverlay.getFillColor();
    int fadeRate = Assets::getInstance().getInt("FlashFadeRate", 5);
    if (flashColor.a > 0) {
        flashColor.a = std::max(0, flashColor.a - fadeRate);
        _flashOverlay.setFillColor(flashColor);
    }

    for (size_t i = 0; i < _impactParticles.size(); i++) {
        _particleVelocities[i].y += _particleGravity * dt.asSeconds();

        _impactParticles[i].move(_particleVelocities[i] * dt.asSeconds());

        sf::Color color = _impactParticles[i].getFillColor();
        int particleFadeRate = Assets::getInstance().getInt("ParticleFadeRate", 2);
        color.a = std::max(0, color.a - particleFadeRate);
        _impactParticles[i].setFillColor(color);
    }

    for (size_t i = 0; i < _impactParticles.size();) {
        if (_impactParticles[i].getFillColor().a == 0) {
            _impactParticles.erase(_impactParticles.begin() + i);
            _particleVelocities.erase(_particleVelocities.begin() + i);
        }
        else {
            i++;
        }
    }

    if (_hitAnimationTime >= _hitAnimationDuration) {
        _isHitAnimation = false;
        _dogSprite.setRotation(0.0f); 
        _gameTimeScale = 1.0f; 
        _screenShake = 0.0f; 

        if (_dogHealth <= 0) {
            _isGameOver = true;
            _backgroundMusic.stop();
            _gameOverMusic.play();
        }
    }
}
void Scene_Game::startVictoryAnimation() {
    _isVictoryAnimation = true;
    _victoryAnimationTime = 0.0f;

    int confettiCount = 200;
    for (int i = 0; i < confettiCount; i++) {
        sf::CircleShape confetti;
        confetti.setRadius(3.0f + (rand() % 5));

        sf::Color color;
        switch (rand() % 5) {
        case 0: color = sf::Color::Red; break;
        case 1: color = sf::Color::Blue; break;
        case 2: color = sf::Color::Green; break;
        case 3: color = sf::Color::Yellow; break;
        case 4: color = sf::Color::Magenta; break;
        }
        confetti.setFillColor(color);

        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float distance = 10.0f + (rand() % 20);
        sf::Vector2f offset(std::cos(angle) * distance, std::sin(angle) * distance);
        confetti.setPosition(_homeSprite.getPosition() + offset);

        float xVel = -100.0f + (rand() % 200);
        float yVel = -300.0f - (rand() % 200); 
        sf::Vector2f velocity(xVel, yVel);

        _confettiParticles.push_back(confetti);
        _confettiVelocities.push_back(velocity);
        _confettiColors.push_back(color);
    }

}

void Scene_Game::updateVictoryAnimation(sf::Time dt) {
    if (!_isVictoryAnimation) return;

    _victoryAnimationTime += dt.asSeconds();

    sf::Vector2f homeDirection = _homeSprite.getPosition() - _dogPosition;
    float length = std::sqrt(homeDirection.x * homeDirection.x + homeDirection.y * homeDirection.y);

    if (length > 5.0f) { 
        homeDirection /= length; 
        _dogPosition += homeDirection * _dogSpeed * 0.5f * dt.asSeconds();
        _dogSprite.setPosition(_dogPosition);

        if (homeDirection.x < 0) {
            _dogSprite.setTextureRect(sf::IntRect(_dogAnimationFrame * 32, 32, 32, 32)); // Left
        }
        else if (homeDirection.x > 0) {
            _dogSprite.setTextureRect(sf::IntRect(_dogAnimationFrame * 32, 64, 32, 32)); // Right
        }
        else if (homeDirection.y < 0) {
            _dogSprite.setTextureRect(sf::IntRect(_dogAnimationFrame * 32, 96, 32, 32)); // Up
        }
        else {
            _dogSprite.setTextureRect(sf::IntRect(_dogAnimationFrame * 32, 0, 32, 32)); // Down
        }

        if (_animationClock.getElapsedTime().asSeconds() > 0.1f) {
            _dogAnimationFrame = (_dogAnimationFrame + 1) % 3;
            _animationClock.restart();
        }
    }
    else {
        if (_animationClock.getElapsedTime().asSeconds() > 0.1f) {
            _dogAnimationFrame = (_dogAnimationFrame + 1) % 3;
            _dogSprite.setTextureRect(sf::IntRect(_dogAnimationFrame * 32, 64, 32, 32)); // Right-facing frames
            _animationClock.restart();
        }
    }

    for (size_t i = 0; i < _confettiParticles.size(); i++) {
        _confettiVelocities[i].y += 200.0f * dt.asSeconds();

        _confettiParticles[i].move(_confettiVelocities[i] * dt.asSeconds());

        float wobble = std::sin(_victoryAnimationTime * 10.0f + i) * 2.0f;
        _confettiParticles[i].move(wobble * dt.asSeconds(), 0);

        if (_victoryAnimationTime > _victoryAnimationDuration * 0.7f) {
            sf::Color color = _confettiColors[i];
            color.a = static_cast<sf::Uint8>(255 * (1.0f - (_victoryAnimationTime - _victoryAnimationDuration * 0.7f) / (_victoryAnimationDuration * 0.3f)));
            _confettiParticles[i].setFillColor(color);
        }
    }

    float pulseRate = 3.0f; 
    _homeGlowValue = (std::sin(_victoryAnimationTime * pulseRate * 2 * 3.14159f) + 1.0f) / 2.0f;
    sf::Uint8 alpha = static_cast<sf::Uint8>(_homeGlowValue * 100);
    _homeGlow.setFillColor(sf::Color(255, 255, 100, alpha));

    float scale = 1.0f + _homeGlowValue * 0.5f;
    _homeGlow.setScale(scale, scale);
    _homeGlow.setPosition(_homeSprite.getPosition());

    if (_victoryAnimationTime >= _victoryAnimationDuration) {
        _isVictoryAnimation = false;
        _isWin = true;
        _backgroundMusic.stop();
        _gameOverMusic.play();
    }
}