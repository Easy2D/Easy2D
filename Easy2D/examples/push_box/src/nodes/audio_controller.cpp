#include "audio_controller.h"

#include "../core/storage.h"

namespace pushbox {

easy2d::Ptr<AudioController> AudioController::create() { return easy2d::makePtr<AudioController>(); }

void AudioController::onEnter() {
  Node::onEnter();

  if (!loaded_) {
    auto& resources = easy2d::Application::instance().resources();

    background_ = resources.loadSound("pushbox_bg", "assets/audio/background.wav");
    manMove_ = resources.loadSound("pushbox_manmove", "assets/audio/manmove.wav");
    boxMove_ = resources.loadSound("pushbox_boxmove", "assets/audio/boxmove.wav");

    if (background_) {
      background_->setLooping(true);
      background_->play();
    }

    loaded_ = true;
  }

  setEnabled(g_SoundOpen);
}

void AudioController::setEnabled(bool enabled) {
  enabled_ = enabled;
  g_SoundOpen = enabled;
  saveSoundOpen(enabled);

  if (!background_) {
    return;
  }

  if (enabled_) {
    background_->resume();
  } else {
    background_->pause();
  }
}

void AudioController::playManMove() {
  if (!enabled_ || !manMove_) {
    return;
  }
  manMove_->play();
}

void AudioController::playBoxMove() {
  if (!enabled_ || !boxMove_) {
    return;
  }
  boxMove_->play();
}

} // namespace pushbox

