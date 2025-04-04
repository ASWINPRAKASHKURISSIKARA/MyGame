#include "Assets.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

void Assets::loadFromFile(const std::string& path) {
    std::ifstream config(path);
    if (config.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        config.close();
        exit(1);
    }

    std::cout << "Loading assets from: " << path << std::endl;
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    std::string line;
    while (std::getline(config, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "Window") {
            int width, height;
            iss >> width >> height;
            _vectorValues["WindowSize"] = sf::Vector2f(width, height);
        }
        else if (token == "WindowTitle") {
            std::string title;
            std::getline(iss >> std::ws, title);
            // Remove quotes if present
            if (title.front() == '"' && title.back() == '"') {
                title = title.substr(1, title.size() - 2);
            }
            _stringValues["WindowTitle"] = title;
        }
        else if (token == "FrameRate") {
            int framerate;
            iss >> framerate;
            _intValues["FrameRate"] = framerate;
        }
        else if (token == "Font") {
            std::string name, fontPath;
            iss >> name >> fontPath;
            sf::Font font;
            if (!font.loadFromFile(fontPath)) {
                std::cerr << "Failed to load font " << fontPath << "\n";
            }
            else {
                _fonts[name] = font;
                logAssetLoaded("font", name, fontPath);
            }
        }
        else if (token == "Texture") {
            std::string name, texturePath;
            iss >> name >> texturePath;
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath)) {
                std::cerr << "Failed to load texture " << texturePath << "\n";
            }
            else {
                _textures[name] = texture;
                logAssetLoaded("texture", name, texturePath);
            }
        }
        else if (token == "Sound") {
            std::string name, soundPath;
            iss >> name >> soundPath;
            sf::SoundBuffer soundBuffer;
            if (!soundBuffer.loadFromFile(soundPath)) {
                std::cerr << "Failed to load sound " << soundPath << "\n";
            }
            else {
                _soundBuffers[name] = soundBuffer;
                logAssetLoaded("sound", name, soundPath);
            }
        }
        else {
            float x, y;
            iss >> x;
            if (!iss.fail()) {
                iss >> y;
                if (!iss.fail()) {
                    _vectorValues[token] = sf::Vector2f(x, y);
                    continue;
                }
            }

            iss.clear();
            iss.seekg(0);
            iss >> token;

            float floatValue;
            iss >> floatValue;
            if (!iss.fail()) {
                _floatValues[token] = floatValue;
                continue;
            }

            iss.clear();
            iss.seekg(0);
            iss >> token;

            int intValue;
            iss >> intValue;
            if (!iss.fail()) {
                _intValues[token] = intValue;
                continue;
            }

            iss.clear();
            iss.seekg(0);
            iss >> token;

            std::string stringValue;
            std::getline(iss >> std::ws, stringValue);
            if (!stringValue.empty()) {
                if (stringValue.front() == '"' && stringValue.back() == '"') {
                    stringValue = stringValue.substr(1, stringValue.size() - 2);
                }
                _stringValues[token] = stringValue;
            }
        }
    }

    config.close();
}



const sf::Texture& Assets::getTexture(const std::string& name) const {
    if (!_textures.contains(name)) {
        std::cerr << "Texture " << name << " not found!\n";
        static sf::Texture defaultTexture;
        return defaultTexture;
    }
    return _textures.at(name);
}

const sf::Font& Assets::getFont(const std::string& name) const {
    if (!_fonts.contains(name)) {
        std::cerr << "Font " << name << " not found!\n";
        static sf::Font defaultFont;

        // Try to load a system font as fallback
#ifdef _WIN32
        if (defaultFont.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            return defaultFont;
        }
#endif

        static sf::Font emptyFont;
        return emptyFont;
    }
    return _fonts.at(name);
}

const sf::SoundBuffer& Assets::getSoundBuffer(const std::string& name) const {
    if (!_soundBuffers.contains(name)) {
        std::cerr << "Sound " << name << " not found!\n";
        static sf::SoundBuffer defaultBuffer;
        return defaultBuffer;
    }
    return _soundBuffers.at(name);
}

float Assets::getFloat(const std::string& name, float defaultValue) const {
    if (!_floatValues.contains(name)) {
        std::cerr << "Float value " << name << " not found, using default: " << defaultValue << "\n";
        return defaultValue;
    }
    return _floatValues.at(name);
}

int Assets::getInt(const std::string& name, int defaultValue) const {
    if (!_intValues.contains(name)) {
        return defaultValue;
    }
    return _intValues.at(name);
}

const std::string& Assets::getString(const std::string& name, const std::string& defaultValue) const {
    if (!_stringValues.contains(name)) {
        std::cerr << "String value " << name << " not found, using default: " << defaultValue << "\n";
        static std::string emptyString;
        emptyString = defaultValue;
        return emptyString;
    }
    return _stringValues.at(name);
}

sf::Vector2f Assets::getVector(const std::string& name, const sf::Vector2f& defaultValue) const {
    if (!_vectorValues.contains(name)) {
        std::cerr << "Vector value " << name << " not found, using default: ("
            << defaultValue.x << ", " << defaultValue.y << ")\n";
        return defaultValue;
    }
    return _vectorValues.at(name);
}
