#pragma once

#include <string>
#include <easy2d/core/types.h>

struct ma_sound;

namespace easy2d {

class AudioEngine;

class Sound {
public:
    ~Sound();

    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;

    bool play();
    void pause();
    void resume();
    void stop();

    bool isPlaying() const;
    bool isPaused() const;

    void setVolume(float volume);
    float getVolume() const { return volume_; }

    void setLooping(bool looping);
    bool isLooping() const;

    void setPitch(float pitch);
    float getPitch() const;

    float getDuration() const;
    float getCursor() const;
    void setCursor(float seconds);

    const std::string& getFilePath() const { return filePath_; }
    const std::string& getName() const { return name_; }

private:
    friend class AudioEngine;

    Sound(const std::string& name, const std::string& filePath, ma_sound* sound);

    std::string name_;
    std::string filePath_;
    ma_sound* sound_ = nullptr;
    float volume_ = 1.0f;
};

} // namespace easy2d
