#pragma once

#include <easy2d/easy2d.h>

namespace pushbox {

class MenuButton;

class StartScene : public easy2d::Scene {
public:
  StartScene();
  void onEnter() override;

private:
  void startNewGame();
  void continueGame();
  void exitGame();

  easy2d::Ptr<MenuButton> resumeBtn_;
  easy2d::Ptr<easy2d::ToggleImageButton> soundBtn_;
  easy2d::Ptr<easy2d::FontAtlas> font_;
};

} // namespace pushbox
