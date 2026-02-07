#include "core/data.h"
#include "core/storage.h"
#include "scenes/start_scene.h"
#include "scenes/play_scene.h"
#include <easy2d/easy2d.h>
#include <filesystem>

static std::filesystem::path getExecutableDir(int argc, char** argv) {
  if (argc <= 0 || argv == nullptr || argv[0] == nullptr) {
    return std::filesystem::current_path();
  }

  std::error_code ec;
  auto exePath = std::filesystem::absolute(argv[0], ec);
  if (ec) {
    return std::filesystem::current_path();
  }
  return exePath.parent_path();
}

static float parseAutoQuitSeconds(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    if (!argv[i]) {
      continue;
    }
    const std::string arg = argv[i];
    const std::string prefix = "--autoquit=";
    if (arg.rfind(prefix, 0) == 0) {
      try {
        return std::stof(arg.substr(prefix.size()));
      } catch (...) {
        return 0.0f;
      }
    }
  }
  return 0.0f;
}

int main(int argc, char** argv) {
  easy2d::Logger::init();
  easy2d::Logger::setLevel(easy2d::LogLevel::Info);

  auto& app = easy2d::Application::instance();

  easy2d::AppConfig config;
  config.title = "推箱子";
  config.width = 640;
  config.height = 480;
  config.vsync = true;
  config.fpsLimit = 0;

  if (!app.init(config)) {
    easy2d::Logger::shutdown();
    return -1;
  }

  const auto exeDir = getExecutableDir(argc, argv);
  auto& resources = app.resources();
  resources.addSearchPath(exeDir.string());
  resources.addSearchPath((exeDir / "assets").string());
  resources.addSearchPath((exeDir.parent_path() / "assets").string());
  resources.addSearchPath((exeDir.parent_path() / "src").string());
  resources.addSearchPath("assets");
  resources.addSearchPath("src");

  pushbox::initStorage(exeDir);
  pushbox::g_CurrentLevel = pushbox::loadCurrentLevel(1);
  if (pushbox::g_CurrentLevel > MAX_LEVEL) {
    pushbox::g_CurrentLevel = 1;
  }
  pushbox::g_SoundOpen = pushbox::loadSoundOpen(true);

  // 进入开始场景（主界面）
  app.enterScene(easy2d::makePtr<pushbox::StartScene>());

  const float autoQuitSeconds = parseAutoQuitSeconds(argc, argv);
  if (autoQuitSeconds > 0.0f) {
    app.timers().addTimer(autoQuitSeconds, [&app]() { app.quit(); });
  }
  app.run();

  app.shutdown();
  easy2d::Logger::shutdown();
  return 0;
}
