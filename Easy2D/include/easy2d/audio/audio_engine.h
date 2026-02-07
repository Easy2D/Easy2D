#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <easy2d/core/types.h>

struct ma_engine;
struct ma_sound;

namespace easy2d {

class Sound;

class AudioEngine {
public:
    static AudioEngine& getInstance();

    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;
    AudioEngine(AudioEngine&&) = delete;
    AudioEngine& operator=(AudioEngine&&) = delete;

    bool initialize();
    void shutdown();

    std::shared_ptr<Sound> loadSound(const std::string& filePath);
    std::shared_ptr<Sound> loadSound(const std::string& name, const std::string& filePath);

    std::shared_ptr<Sound> getSound(const std::string& name);
    void unloadSound(const std::string& name);
    void unloadAllSounds();

    void setMasterVolume(float volume);
    float getMasterVolume() const;

    void pauseAll();
    void resumeAll();
    void stopAll();

    ma_engine* getEngine() const { return engine_; }

private:
    AudioEngine() = default;
    ~AudioEngine();

    ma_engine* engine_ = nullptr;
    // 使用 shared_ptr 存储，确保在 AudioEngine 销毁前所有 Sound 都被销毁
    std::unordered_map<std::string, std::shared_ptr<Sound>> sounds_;
    float masterVolume_ = 1.0f;
};

} // namespace easy2d
