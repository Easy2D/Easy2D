#include "audio_context.h"

namespace pushbox {

static easy2d::WeakPtr<AudioController> g_audioController;

void setAudioController(const easy2d::Ptr<AudioController>& controller) {
  g_audioController = controller;
}

easy2d::Ptr<AudioController> getAudioController() { return g_audioController.lock(); }

} // namespace pushbox

