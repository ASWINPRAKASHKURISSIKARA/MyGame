#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

using String = std::string;

class MusicPlayer {
private:
    std::map<String, String> m_filenames;
    sf::Music m_music;
    float m_volume = 100.f;

    MusicPlayer();

    MusicPlayer(const MusicPlayer&) = delete;
    MusicPlayer& operator=(const MusicPlayer&) = delete;

public:
    static MusicPlayer& getInstance();

    void addSong(const std::string& name, const std::string& path);
    void play(String theme);
    void stop();
    void setPaused(bool paused);
    void setVolume(float volume);
};
