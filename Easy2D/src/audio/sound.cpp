#include <easy2d/audio/sound.h>
#include <miniaudio/miniaudio.h>

namespace easy2d {

Sound::Sound(const std::string& name, const std::string& filePath, ma_sound* sound)
    : name_(name), filePath_(filePath), sound_(sound) {
}

Sound::~Sound() {
    if (sound_) {
        ma_sound_uninit(sound_);
        delete sound_;
        sound_ = nullptr;
    }
}

bool Sound::play() {
    if (!sound_) {
        return false;
    }

    // 如果声音正在播放，先停止并重置到开头
    if (ma_sound_is_playing(sound_)) {
        ma_sound_stop(sound_);
        ma_sound_seek_to_pcm_frame(sound_, 0);
    }

    ma_result result = ma_sound_start(sound_);
    return result == MA_SUCCESS;
}

void Sound::pause() {
    if (sound_) {
        ma_sound_stop(sound_);
    }
}

void Sound::resume() {
    play();
}

void Sound::stop() {
    if (sound_) {
        ma_sound_stop(sound_);
        ma_sound_seek_to_pcm_frame(sound_, 0);
    }
}

bool Sound::isPlaying() const {
    if (!sound_) {
        return false;
    }
    return ma_sound_is_playing(sound_);
}

bool Sound::isPaused() const {
    if (!sound_) {
        return false;
    }
    return !ma_sound_is_playing(sound_) && !ma_sound_at_end(sound_);
}

void Sound::setVolume(float volume) {
    volume_ = volume;
    if (sound_) {
        ma_sound_set_volume(sound_, volume);
    }
}

void Sound::setLooping(bool looping) {
    if (sound_) {
        ma_sound_set_looping(sound_, looping);
    }
}

bool Sound::isLooping() const {
    if (!sound_) {
        return false;
    }
    return ma_sound_is_looping(sound_);
}

void Sound::setPitch(float pitch) {
    if (sound_) {
        ma_sound_set_pitch(sound_, pitch);
    }
}

float Sound::getPitch() const {
    if (!sound_) {
        return 1.0f;
    }
    return ma_sound_get_pitch(sound_);
}

float Sound::getDuration() const {
    if (!sound_) {
        return 0.0f;
    }

    float lengthInSeconds;
    ma_result result = ma_sound_get_length_in_seconds(sound_, &lengthInSeconds);
    if (result != MA_SUCCESS) {
        return 0.0f;
    }
    return lengthInSeconds;
}

float Sound::getCursor() const {
    if (!sound_) {
        return 0.0f;
    }

    float cursorInSeconds;
    ma_result result = ma_sound_get_cursor_in_seconds(sound_, &cursorInSeconds);
    if (result != MA_SUCCESS) {
        return 0.0f;
    }
    return cursorInSeconds;
}

void Sound::setCursor(float seconds) {
    if (sound_) {
        ma_sound_seek_to_pcm_frame(sound_, static_cast<ma_uint64>(seconds * ma_engine_get_sample_rate(nullptr)));
    }
}

} // namespace easy2d
