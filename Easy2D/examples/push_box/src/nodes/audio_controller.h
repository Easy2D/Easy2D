#pragma once

#include "../core/data.h"
#include <easy2d/easy2d.h>

namespace pushbox {

class AudioController : public easy2d::Node {
public:
  static easy2d::Ptr<AudioController> create();

  void onEnter() override;

  void setEnabled(bool enabled);
  bool isEnabled() const { return enabled_; }

  void playManMove();
  void playBoxMove();

private:
  bool loaded_ = false;
  bool enabled_ = true;

  easy2d::Ptr<easy2d::Sound> background_;
  easy2d::Ptr<easy2d::Sound> manMove_;
  easy2d::Ptr<easy2d::Sound> boxMove_;
};

} // namespace pushbox

