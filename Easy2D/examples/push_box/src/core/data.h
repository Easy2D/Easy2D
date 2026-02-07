#pragma once

#define MAX_LEVEL 8

namespace pushbox {

enum TYPE { Empty, Wall, Ground, Box, Man };

struct Piece {
  TYPE type;
  bool isPoint;
};

struct Map {
  int width;
  int height;
  int roleX;
  int roleY;
  Piece value[12][12];
};

extern Map g_Maps[MAX_LEVEL];
extern int g_CurrentLevel;
extern bool g_SoundOpen;
extern int g_Direct;
extern bool g_Pushing;

} // namespace pushbox

