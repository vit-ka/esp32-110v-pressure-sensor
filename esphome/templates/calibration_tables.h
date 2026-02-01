#pragma once
#include <cstring>

struct CalibrationTable {
  const char* id;
  const float (*points)[2];
  int size;
};

// 0-0.8MPa probe 
static const float cal_0_08Mpa[][2] = {
  {0.000f,   0.0f},
  {0.110f,  10.0f},
  {0.260f,  20.0f},
  {0.410f,  30.0f},
  {0.560f,  40.0f},
  {0.710f,  50.0f},
  {0.860f,  60.0f},
  {1.010f,  70.0f},
  {1.160f,  80.0f},
  {1.310f,  90.0f},
  {1.460f, 100.0f},
  {1.610f, 110.0f},
  {1.760f, 120.0f}
};

// O-1.0MPa probe
static const float cal_0_10MPa[][2] = {
  {0.000f,   0.0f},
  {0.120f,  10.0f},
  {0.230f,  20.0f},
  {0.340f,  30.0f},
  {0.450f,  40.0f},
  {0.560f,  50.0f},
  {0.680f,  60.0f},
  {0.800f,  70.0f},
  {0.920f,  80.0f},
  {1.040f,  90.0f},
  {1.160f, 100.0f}
};

static const CalibrationTable CALIBRATION_TABLES[] = {
  {"0-0.8MPa",  cal_0_08Mpa,  sizeof(cal_0_08Mpa)  / sizeof(cal_0_08Mpa[0])},
  {"0-1.0MPa", cal_0_10MPa, sizeof(cal_0_10MPa) / sizeof(cal_0_10MPa[0])},
};

static const int CALIBRATION_TABLE_COUNT = sizeof(CALIBRATION_TABLES) / sizeof(CALIBRATION_TABLES[0]);

inline const CalibrationTable* find_calibration(const char* id) {
  for (int i = 0; i < CALIBRATION_TABLE_COUNT; i++) {
    if (strcmp(CALIBRATION_TABLES[i].id, id) == 0) {
      return &CALIBRATION_TABLES[i];
    }
  }
  return nullptr;
}
