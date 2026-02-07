#include "easy2d/audio/audio_engine.h"
#include "easy2d/audio/sound.h"
#include "easy2d/utils/logger.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

namespace easy2d {

AudioEngine& AudioEngine::getInstance() {
    static AudioEngine instance;
    return instance;
}

AudioEngine::~AudioEngine() {
    shutdown();
}

bool AudioEngine::initialize() {
    if (engine_) {
        return true;
    }

    engine_ = new ma_engine();
    ma_result result = ma_engine_init(nullptr, engine_);

    if (result != MA_SUCCESS) {
        E2D_LOG_ERROR("Failed to initialize audio engine");
        delete engine_;
        engine_ = nullptr;
        return false;
    }

    E2D_LOG_INFO("AudioEngine initialized successfully");
    return true;
}

void AudioEngine::shutdown() {
    if (!engine_) {
        return;
    }

    unloadAllSounds();

    ma_engine_uninit(engine_);
    delete engine_;
    engine_ = nullptr;

    E2D_LOG_INFO("AudioEngine shutdown");
}

std::shared_ptr<Sound> AudioEngine::loadSound(const std::string& filePath) {
    return loadSound(filePath, filePath);
}

std::shared_ptr<Sound> AudioEngine::loadSound(const std::string& name, const std::string& filePath) {
    if (!engine_) {
        E2D_LOG_ERROR("AudioEngine not initialized");
        return nullptr;
    }

    // 检查是否已存在
    auto it = sounds_.find(name);
    if (it != sounds_.end()) {
        return it->second;
    }

    ma_sound* maSound = new ma_sound();
    ma_result result = ma_sound_init_from_file(engine_, filePath.c_str(), 0, nullptr, nullptr, maSound);

    if (result != MA_SUCCESS) {
        E2D_LOG_ERROR("Failed to load sound: {}", filePath);
        delete maSound;
        return nullptr;
    }

    auto sound = std::shared_ptr<Sound>(new Sound(name, filePath, maSound));
    sounds_[name] = sound;

    E2D_LOG_DEBUG("Loaded sound: {}", filePath);
    return sound;
}

std::shared_ptr<Sound> AudioEngine::getSound(const std::string& name) {
    auto it = sounds_.find(name);
    if (it != sounds_.end()) {
        return it->second;
    }
    return nullptr;
}

void AudioEngine::unloadSound(const std::string& name) {
    auto it = sounds_.find(name);
    if (it != sounds_.end()) {
        sounds_.erase(it);
        E2D_LOG_DEBUG("Unloaded sound: {}", name);
    }
}

void AudioEngine::unloadAllSounds() {
    // 先停止所有声音
    stopAll();
    
    // 强制销毁所有 Sound 对象（通过重置 weak_ptr 并让 shared_ptr 自然释放）
    // 注意：这要求所有持有 Sound 的地方都释放引用
    sounds_.clear();
    
    E2D_LOG_DEBUG("Unloaded all sounds");
}

void AudioEngine::setMasterVolume(float volume) {
    masterVolume_ = volume;
    if (engine_) {
        ma_engine_set_volume(engine_, volume);
    }
}

float AudioEngine::getMasterVolume() const {
    return masterVolume_;
}

void AudioEngine::pauseAll() {
    if (engine_) {
        ma_engine_stop(engine_);
    }
}

void AudioEngine::resumeAll() {
    if (engine_) {
        ma_engine_start(engine_);
    }
}

void AudioEngine::stopAll() {
    for (auto& pair : sounds_) {
        if (pair.second) {
            pair.second->stop();
        }
    }
}

} // namespace easy2d
