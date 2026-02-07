#include "start_scene.h"

#include "../core/audio_context.h"
#include "../core/data.h"
#include "../nodes/audio_controller.h"
#include "../scenes/play_scene.h"
#include "../ui/menu_button.h"
#include <easy2d/easy2d.h>

namespace pushbox {

StartScene::StartScene() {
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

void StartScene::onEnter() {
  Scene::onEnter();

  auto& app = easy2d::Application::instance();
  auto& resources = app.resources();
  setBackgroundColor(easy2d::Colors::Black);

  if (getChildren().empty()) {
    auto audioNode = AudioController::create();
    audioNode->setName("audio_controller");
    addChild(audioNode);
    setAudioController(audioNode);

    auto bgTex = resources.loadTexture("assets/images/start.jpg");
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

    font_ = loadMenuFont();
    if (!font_) {
      return;
    }

    auto startBtn = MenuButton::create(font_, "新游戏", [this]() { startNewGame(); });
    startBtn->setPosition(app.getConfig().width / 2.0f, 260.0f);
    addChild(startBtn);

    resumeBtn_ = MenuButton::create(font_, "继续关卡", [this]() { continueGame(); });
    resumeBtn_->setPosition(app.getConfig().width / 2.0f, 300.0f);
    addChild(resumeBtn_);

    auto exitBtn = MenuButton::create(font_, "退出", [this]() { exitGame(); });
    exitBtn->setPosition(app.getConfig().width / 2.0f, 340.0f);
    addChild(exitBtn);

    auto soundOn = resources.loadTexture("assets/images/soundon.png");
    auto soundOff = resources.loadTexture("assets/images/soundoff.png");
    if (soundOn && soundOff) {
      soundBtn_ = easy2d::ToggleImageButton::create();
      soundBtn_->setStateImages(soundOff, soundOn);
      soundBtn_->setCustomSize(static_cast<float>(soundOn->getWidth()),
                               static_cast<float>(soundOn->getHeight()));
      soundBtn_->setBorder(easy2d::Colors::Transparent, 0.0f);
      soundBtn_->setPosition(50.0f, 50.0f);
      soundBtn_->setOnStateChange([](bool on) {
        if (auto audio = getAudioController()) {
          audio->setEnabled(on);
        }
      });
      addChild(soundBtn_);
    }
  }

  if (resumeBtn_) {
    resumeBtn_->setEnabled(g_CurrentLevel != 1);
  }

  if (soundBtn_) {
    soundBtn_->setOn(g_SoundOpen);
  }
}

void StartScene::startNewGame() {
  easy2d::Application::instance().scenes().replaceScene(
      easy2d::makePtr<PlayScene>(1), easy2d::TransitionType::Fade, 0.25f);
}

void StartScene::continueGame() {
  easy2d::Application::instance().scenes().replaceScene(
      easy2d::makePtr<PlayScene>(g_CurrentLevel), easy2d::TransitionType::Fade, 0.25f);
}

void StartScene::exitGame() { easy2d::Application::instance().quit(); }

} // namespace pushbox
