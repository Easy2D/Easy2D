#include "success_scene.h"

#include "../ui/menu_button.h"
#include <easy2d/app/application.h>
#include <easy2d/resource/resource_manager.h>
#include <easy2d/scene/scene_manager.h>
#include <easy2d/scene/sprite.h>

namespace pushbox {

SuccessScene::SuccessScene() {
  // 设置视口大小为窗口尺寸
  auto& app = easy2d::Application::instance();
  auto& config = app.getConfig();
  setViewportSize(static_cast<float>(config.width), static_cast<float>(config.height));
}

static easy2d::Ptr<easy2d::FontAtlas> loadMenuFont() {
  auto& resources = easy2d::Application::instance().resources();
  const char* candidates[] = {
      "C:/Windows/Fonts/simsun.ttc",
      "C:/Windows/Fonts/simhei.ttf",
      "C:/Windows/Fonts/segoeui.ttf",
      "C:/Windows/Fonts/arial.ttf",
  };

  for (auto* path : candidates) {
    auto font = resources.loadFont(path, 28);
    if (font) {
      return font;
    }
  }
  return nullptr;
}

void SuccessScene::onEnter() {
  Scene::onEnter();

  auto& app = easy2d::Application::instance();
  auto& resources = app.resources();
  setBackgroundColor(easy2d::Colors::Black);

  if (getChildren().empty()) {
    auto bgTex = resources.loadTexture("assets/images/success.jpg");
    if (bgTex) {
      auto background = easy2d::Sprite::create(bgTex);
      background->setAnchor(0.0f, 0.0f);
      background->setPosition(0.0f, 0.0f);
      float sx = static_cast<float>(app.getConfig().width) / static_cast<float>(bgTex->getWidth());
      float sy =
          static_cast<float>(app.getConfig().height) / static_cast<float>(bgTex->getHeight());
      background->setScale(sx, sy);
      addChild(background);
    }

    auto font = loadMenuFont();
    if (font) {
      auto backBtn = MenuButton::create(font, "回主菜单", []() {
        auto& scenes = easy2d::Application::instance().scenes();
        scenes.popScene(easy2d::TransitionType::Fade, 0.2f);
        scenes.popScene(easy2d::TransitionType::Fade, 0.2f);
      });
      backBtn->setPosition(app.getConfig().width / 2.0f, 350.0f);
      addChild(backBtn);
    }
  }
}

} // namespace pushbox
