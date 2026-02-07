#pragma once

#include <easy2d/easy2d.h>

namespace pushbox {

class AudioController;

void setAudioController(const easy2d::Ptr<AudioController>& controller);
easy2d::Ptr<AudioController> getAudioController();

} // namespace pushbox

