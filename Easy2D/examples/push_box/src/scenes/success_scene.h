#pragma once

#include <easy2d/easy2d.h>

namespace pushbox {

class SuccessScene : public easy2d::Scene {
public:
  SuccessScene();
  void onEnter() override;
};

} // namespace pushbox
