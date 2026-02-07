#pragma once

#include <filesystem>

namespace pushbox {

void initStorage(const std::filesystem::path& baseDir);

int loadCurrentLevel(int defaultValue = 1);
void saveCurrentLevel(int level);

bool loadSoundOpen(bool defaultValue = true);
void saveSoundOpen(bool open);

int loadBestStep(int level, int defaultValue = 0);
void saveBestStep(int level, int step);

std::filesystem::path storageFilePath();

} // namespace pushbox

