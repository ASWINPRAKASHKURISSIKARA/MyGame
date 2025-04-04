#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>
#include <iostream>


class Assets {
private:
    std::map<std::string, sf::Texture> _textures;
    std::map<std::string, sf::Font> _fonts;
    std::map<std::string, sf::SoundBuffer> _soundBuffers;
    std::map<std::string, float> _floatValues;
    std::map<std::string, int> _intValues;
    std::map<std::string, std::string> _stringValues;
    std::map<std::string, sf::Vector2f> _vectorValues;

    Assets() = default;

    void logAssetLoaded(const std::string& type, const std::string& name, const std::string& path) const {
        std::cout << "Loaded " << type << ": " << name << " from " << path << std::endl;

    }

public:
    static Assets& getInstance() {
        static Assets instance;
        return instance;
    }

    void loadFromFile(const std::string& path);

    const sf::Texture& getTexture(const std::string& name) const;
    const sf::Font& getFont(const std::string& name) const;
    const sf::SoundBuffer& getSoundBuffer(const std::string& name) const;

    float getFloat(const std::string& name, float defaultValue = 0.0f) const;
    int getInt(const std::string& name, int defaultValue = 0) const;
    const std::string& getString(const std::string& name, const std::string& defaultValue = "") const;
    sf::Vector2f getVector(const std::string& name, const sf::Vector2f& defaultValue = { 0, 0 }) const;
};
