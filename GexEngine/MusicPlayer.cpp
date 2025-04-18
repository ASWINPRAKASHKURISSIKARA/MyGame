#include "MusicPlayer.h"
#include <stdexcept>

MusicPlayer::MusicPlayer() {
    m_filenames["background"] = "../assets/backmusic.mp3";
    m_filenames["gameover"] = "../assets/gameover.mp3";
    m_filenames["win"] = "../assets/win.mp3";
}

void MusicPlayer::addSong(const std::string& name, const std::string& path) {
    m_filenames[name] = path;
}

MusicPlayer& MusicPlayer::getInstance() {
    static MusicPlayer instance;
    return instance;
}

void MusicPlayer::play(String theme) {
    if (!m_music.openFromFile(m_filenames[theme]))
        throw std::runtime_error("Music could not open file");

    m_music.setVolume(m_volume);
    m_music.setLoop(true);
    m_music.play();
}

void MusicPlayer::stop() {
    m_music.stop();
}

void MusicPlayer::setPaused(bool paused) {
    if (paused)
        m_music.pause();
    else
        m_music.play();
}

void MusicPlayer::setVolume(float volume) {
    m_volume = volume;
    m_music.setVolume(m_volume);
}
