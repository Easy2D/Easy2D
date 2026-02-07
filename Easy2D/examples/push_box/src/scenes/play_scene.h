#pragma once

#include "../core/data.h"
#include <easy2d/easy2d.h>

namespace pushbox {

class PlayScene : public easy2d::Scene {
public:
  explicit PlayScene(int level);

  void onEnter() override;
  void onUpdate(float dt) override;

private:
  void flush();
  void setLevel(int level);
  void setStep(int step);
  void move(int dx, int dy, int direct);
  void gameOver();

  int step_ = 0;
  Map map_{};

  easy2d::Ptr<easy2d::FontAtlas> font28_;
  easy2d::Ptr<easy2d::FontAtlas> font20_;

  easy2d::Ptr<easy2d::Text> levelText_;
  easy2d::Ptr<easy2d::Text> stepText_;
  easy2d::Ptr<easy2d::Text> bestText_;
  easy2d::Ptr<easy2d::Node> mapLayer_;

  easy2d::Ptr<easy2d::ToggleImageButton> soundBtn_;

  easy2d::Ptr<easy2d::Texture> texWall_;
  easy2d::Ptr<easy2d::Texture> texPoint_;
  easy2d::Ptr<easy2d::Texture> texFloor_;
  easy2d::Ptr<easy2d::Texture> texBox_;
  easy2d::Ptr<easy2d::Texture> texBoxInPoint_;

  easy2d::Ptr<easy2d::Texture> texMan_[5];
  easy2d::Ptr<easy2d::Texture> texManPush_[5];
};

} // namespace pushbox
