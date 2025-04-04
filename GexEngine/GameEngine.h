#pragma once

#include "Assets.h"
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

class Scene;
class Command;

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class GameEngine
{
private:
    sf::RenderWindow            _window;
    std::string                 _currentScene;
    SceneMap                    _sceneMap;
    size_t                      _simulationSpeed{ 1 };
    bool                        _running{ true };
    sf::Time                    _frameTime;  

    // stats
    sf::Text                    _statisticsText;
    sf::Time                    _statisticsUpdateTime{ sf::Time::Zero };
    unsigned int                _statisticsNumFrames{ 0 };

public:
    GameEngine(const std::string& path);

    void                    init(const std::string& path);
    void                    update();
    void                    sUserInput();
    std::shared_ptr<Scene>  currentScene();

    void                    changeScene(const std::string& sceneName,
        std::shared_ptr<Scene> scene,
        bool endCurrentScene = false);
    void                    quit();
    void                    run();
    void                    quitLevel();
    void                    backLevel();

    sf::RenderWindow& window();
    sf::Vector2f            windowSize() const;
    bool                    isRunning();

    void                    loadConfigFromFile(const std::string& path,
        unsigned int& width, unsigned int& height) const;
};
