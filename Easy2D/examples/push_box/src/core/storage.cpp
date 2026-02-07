#include "storage.h"

#include <easy2d/utils/data.h>
#include <string>

namespace pushbox {

static easy2d::DataStore g_store;
static std::filesystem::path g_filePath;
static bool g_loaded = false;

static void ensureLoaded() {
  if (g_loaded) {
    return;
  }
  if (!g_filePath.empty()) {
    g_store.load(g_filePath.string());
  }
  g_loaded = true;
}

void initStorage(const std::filesystem::path& baseDir) {
  g_filePath = baseDir / "pushbox.ini";
  g_store.load(g_filePath.string());
  g_loaded = true;
}

int loadCurrentLevel(int defaultValue) {
  ensureLoaded();
  int level = g_store.getInt("game", "level", defaultValue);
  if (level < 1) {
    level = 1;
  }
  return level;
}

void saveCurrentLevel(int level) {
  ensureLoaded();
  g_store.setInt("game", "level", level);
  if (!g_filePath.empty()) {
    g_store.save(g_filePath.string());
  }
}

bool loadSoundOpen(bool defaultValue) {
  ensureLoaded();
  return g_store.getBool("game", "sound", defaultValue);
}

void saveSoundOpen(bool open) {
  ensureLoaded();
  g_store.setBool("game", "sound", open);
  if (!g_filePath.empty()) {
    g_store.save(g_filePath.string());
  }
}

int loadBestStep(int level, int defaultValue) {
  ensureLoaded();
  std::string key = "level" + std::to_string(level);
  return g_store.getInt("best", key, defaultValue);
}

void saveBestStep(int level, int step) {
  ensureLoaded();
  std::string key = "level" + std::to_string(level);
  g_store.setInt("best", key, step);
  if (!g_filePath.empty()) {
    g_store.save(g_filePath.string());
  }
}

std::filesystem::path storageFilePath() { return g_filePath; }

} // namespace pushbox

