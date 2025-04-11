#pragma once
#include <SFML/Audio.hpp>
#include <list>
#include <string>

using String = std::string;

class SoundPlayer {
private:
    std::list<sf::Sound> m_sounds;

    SoundPlayer();

    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

public:
    static SoundPlayer& getInstance();

    void play(String effect);
    void play(String effect, sf::Vector2f position);
    void removeStoppedSounds();
    void setListnerPosition(sf::Vector2f position);
    void setListnerDirection(sf::Vector2f position);
    sf::Vector2f getListnerPosition() const;
    bool isEmpty() const;
};
