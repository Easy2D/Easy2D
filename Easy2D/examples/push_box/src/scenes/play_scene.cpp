#include "play_scene.h"

#include "../core/audio_context.h"
#include "../core/storage.h"
#include "../nodes/audio_controller.h"
#include "start_scene.h"
#include "success_scene.h"
#include <easy2d/easy2d.h>

namespace pushbox {

static easy2d::Ptr<easy2d::FontAtlas> loadFont(int size) {
  auto& resources = easy2d::Application::instance().resources();
  const char* candidates[] = {
      "C:/Windows/Fonts/simsun.ttc",
      "C:/Windows/Fonts/simhei.ttf",
      "C:/Windows/Fonts/segoeui.ttf",
      "C:/Windows/Fonts/arial.ttf",
  };

  for (auto* path : candidates) {
    auto font = resources.loadFont(path, size);
    if (font) {
      return font;
    }
  }
  return nullptr;
}

PlayScene::PlayScene(int level) {
  setBackgroundColor(easy2d::Colors::Black);

  // 设置视口大小为窗口尺寸
  auto& app = easy2d::Application::instance();
  auto& config = app.getConfig();
  setViewportSize(static_cast<float>(config.width), static_cast<float>(config.height));

  auto& resources = app.resources();

  E2D_LOG_INFO("PlayScene: Loading textures...");

  texWall_ = resources.loadTexture("assets/images/wall.gif");
  E2D_LOG_INFO("wall texture: {}", texWall_ ? "OK" : "FAILED");

  texPoint_ = resources.loadTexture("assets/images/point.gif");
  texFloor_ = resources.loadTexture("assets/images/floor.gif");
  texBox_ = resources.loadTexture("assets/images/box.gif");
  texBoxInPoint_ = resources.loadTexture("assets/images/boxinpoint.gif");

  if (!texWall_ || !texFloor_ || !texBox_ || !texBoxInPoint_) {
      E2D_LOG_ERROR("PlayScene: Failed to load basic textures!");
  }

  texMan_[1] = resources.loadTexture("assets/images/player/manup.gif");
  texMan_[2] = resources.loadTexture("assets/images/player/mandown.gif");
  texMan_[3] = resources.loadTexture("assets/images/player/manleft.gif");
  texMan_[4] = resources.loadTexture("assets/images/player/manright.gif");

  texManPush_[1] = resources.loadTexture("assets/images/player/manhandup.gif");
  texManPush_[2] = resources.loadTexture("assets/images/player/manhanddown.gif");
  texManPush_[3] = resources.loadTexture("assets/images/player/manhandleft.gif");
  texManPush_[4] = resources.loadTexture("assets/images/player/manhandright.gif");

  font28_ = loadFont(28);
  font20_ = loadFont(20);

  levelText_ = easy2d::Text::create("", font28_);
  levelText_->setPosition(520.0f, 30.0f);
  levelText_->setTextColor(easy2d::Colors::White);
  addChild(levelText_);

  stepText_ = easy2d::Text::create("", font20_);
  stepText_->setPosition(520.0f, 100.0f);
  stepText_->setTextColor(easy2d::Colors::White);
  addChild(stepText_);

  bestText_ = easy2d::Text::create("", font20_);
  bestText_->setPosition(520.0f, 140.0f);
  bestText_->setTextColor(easy2d::Colors::White);
  addChild(bestText_);

  auto exitText = easy2d::Text::create("按ESC返回", font20_);
  exitText->setPosition(520.0f, 250.0f);
  exitText->setTextColor(easy2d::Colors::White);
  addChild(exitText);

  auto restartText = easy2d::Text::create("按回车重开", font20_);
  restartText->setPosition(520.0f, 290.0f);
  restartText->setTextColor(easy2d::Colors::White);
  addChild(restartText);

  auto soundOn = resources.loadTexture("assets/images/soundon.png");
  auto soundOff = resources.loadTexture("assets/images/soundoff.png");
  if (soundOn && soundOff) {
    soundBtn_ = easy2d::ToggleImageButton::create();
    soundBtn_->setStateImages(soundOff, soundOn);
    soundBtn_->setCustomSize(static_cast<float>(soundOn->getWidth()),
                             static_cast<float>(soundOn->getHeight()));
    soundBtn_->setBorder(easy2d::Colors::Transparent, 0.0f);
    soundBtn_->setPosition(560.0f, 360.0f);
    soundBtn_->setOnStateChange([](bool on) {
      if (auto audio = getAudioController()) {
        audio->setEnabled(on);
      }
    });
    addChild(soundBtn_);
  }

  mapLayer_ = easy2d::makePtr<easy2d::Node>();
  mapLayer_->setAnchor(0.0f, 0.0f);
  mapLayer_->setPosition(0.0f, 0.0f);
  addChild(mapLayer_);

  // 创建音频控制器
  auto audioNode = AudioController::create();
  audioNode->setName("AudioController");
  addChild(audioNode);
  setAudioController(audioNode);

  setLevel(level);
}

void PlayScene::onEnter() {
  Scene::onEnter();
  if (soundBtn_) {
    soundBtn_->setOn(g_SoundOpen);
  }
}

void PlayScene::onUpdate(float dt) {
  Scene::onUpdate(dt);

  auto& app = easy2d::Application::instance();
  auto& input = app.input();

  if (input.isKeyPressed(easy2d::Key::Escape)) {
    app.scenes().replaceScene(
        easy2d::makePtr<StartScene>(), easy2d::TransitionType::Fade, 0.2f);
    return;
  }

  if (input.isKeyPressed(easy2d::Key::Enter)) {
    setLevel(g_CurrentLevel);
    return;
  }

  if (input.isKeyPressed(easy2d::Key::Up)) {
    move(0, -1, 1);
    flush();
  } else if (input.isKeyPressed(easy2d::Key::Down)) {
    move(0, 1, 2);
    flush();
  } else if (input.isKeyPressed(easy2d::Key::Left)) {
    move(-1, 0, 3);
    flush();
  } else if (input.isKeyPressed(easy2d::Key::Right)) {
    move(1, 0, 4);
    flush();
  } else {
    return;
  }

  for (int i = 0; i < map_.width; i++) {
    for (int j = 0; j < map_.height; j++) {
      Piece p = map_.value[j][i];
      if (p.type == TYPE::Box && p.isPoint == false) {
        return;
      }
    }
  }

  gameOver();
}

void PlayScene::flush() {
  mapLayer_->removeAllChildren();

  int tileW = texFloor_ ? texFloor_->getWidth() : 32;
  int tileH = texFloor_ ? texFloor_->getHeight() : 32;

  float offsetX = static_cast<float>((12 - map_.width) / 2) * tileW;
  float offsetY = static_cast<float>((12 - map_.height) / 2) * tileH;

  for (int i = 0; i < map_.width; i++) {
    for (int j = 0; j < map_.height; j++) {
      Piece piece = map_.value[j][i];

      easy2d::Ptr<easy2d::Texture> tex;

      if (piece.type == TYPE::Wall) {
        tex = texWall_;
      } else if (piece.type == TYPE::Ground && piece.isPoint) {
        tex = texPoint_;
      } else if (piece.type == TYPE::Ground) {
        tex = texFloor_;
      } else if (piece.type == TYPE::Box && piece.isPoint) {
        tex = texBoxInPoint_;
      } else if (piece.type == TYPE::Box) {
        tex = texBox_;
      } else if (piece.type == TYPE::Man && g_Pushing) {
        tex = texManPush_[g_Direct];
      } else if (piece.type == TYPE::Man) {
        tex = texMan_[g_Direct];
      } else {
        continue;
      }

      if (!tex) {
        continue;
      }

      auto sprite = easy2d::Sprite::create(tex);
      sprite->setAnchor(0.0f, 0.0f);
      sprite->setPosition(offsetX + static_cast<float>(i * tileW),
                          offsetY + static_cast<float>(j * tileH));
      mapLayer_->addChild(sprite);
    }
  }
}

void PlayScene::setLevel(int level) {
  g_CurrentLevel = level;
  saveCurrentLevel(g_CurrentLevel);

  if (levelText_) {
    levelText_->setText("第" + std::to_string(level) + "关");
  }

  setStep(0);

  int bestStep = loadBestStep(level, 0);
  if (bestText_) {
    if (bestStep != 0) {
      bestText_->setText("最佳" + std::to_string(bestStep) + "步");
    } else {
      bestText_->setText("");
    }
  }

  map_ = g_Maps[level - 1];
  g_Direct = 2;
  g_Pushing = false;
  flush();
}

void PlayScene::setStep(int step) {
  step_ = step;
  if (stepText_) {
    stepText_->setText("当前" + std::to_string(step) + "步");
  }
}

void PlayScene::move(int dx, int dy, int direct) {
  int targetX = dx + map_.roleX;
  int targetY = dy + map_.roleY;
  g_Direct = direct;

  if (targetX < 0 || targetX >= map_.width || targetY < 0 || targetY >= map_.height) {
    return;
  }

  if (map_.value[targetY][targetX].type == TYPE::Wall) {
    return;
  }

  if (map_.value[targetY][targetX].type == TYPE::Ground) {
    g_Pushing = false;
    map_.value[map_.roleY][map_.roleX].type = TYPE::Ground;
    map_.value[targetY][targetX].type = TYPE::Man;
    if (auto audio = getAudioController()) {
      audio->playManMove();
    }
  } else if (map_.value[targetY][targetX].type == TYPE::Box) {
    g_Pushing = true;

    int boxX = 0;
    int boxY = 0;
    switch (g_Direct) {
    case 1:
      boxX = targetX;
      boxY = targetY - 1;
      break;
    case 2:
      boxX = targetX;
      boxY = targetY + 1;
      break;
    case 3:
      boxX = targetX - 1;
      boxY = targetY;
      break;
    case 4:
      boxX = targetX + 1;
      boxY = targetY;
      break;
    default:
      return;
    }

    if (boxX < 0 || boxX >= map_.width || boxY < 0 || boxY >= map_.height) {
      return;
    }

    if (map_.value[boxY][boxX].type == TYPE::Wall || map_.value[boxY][boxX].type == TYPE::Box) {
      return;
    }

    map_.value[boxY][boxX].type = TYPE::Box;
    map_.value[targetY][targetX].type = TYPE::Man;
    map_.value[map_.roleY][map_.roleX].type = TYPE::Ground;

    if (auto audio = getAudioController()) {
      audio->playBoxMove();
    }
  } else {
    return;
  }

  map_.roleX = targetX;
  map_.roleY = targetY;
  setStep(step_ + 1);
}

void PlayScene::gameOver() {
  int bestStep = loadBestStep(g_CurrentLevel, 0);
  if (bestStep == 0 || step_ < bestStep) {
    saveBestStep(g_CurrentLevel, step_);
  }

  if (g_CurrentLevel == MAX_LEVEL) {
    easy2d::Application::instance().scenes().pushScene(easy2d::makePtr<SuccessScene>(),
                                                      easy2d::TransitionType::Fade, 0.25f);
    return;
  }

  setLevel(g_CurrentLevel + 1);
}

} // namespace pushbox
