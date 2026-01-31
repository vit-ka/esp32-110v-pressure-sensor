#pragma once
#include <cstring>

struct CalibrationTable {
  const char* id;
  const float (*points)[2];
  int size;
};

// first_batch
static const float cal_first_batch[][2] = {
  {0.000f,  0.0f},
  {0.086f, 10.0f},
  {0.244f, 20.0f},
  {0.394f, 30.0f},
  {0.553f, 40.0f},
  {0.709f, 50.0f},
  {0.888f, 60.0f},
  {1.035f, 70.0f},
  {1.255f, 80.0f},
  {1.496f, 90.0f},
  {1.612f, 99.9f}
};

// second_batch
static const float cal_second_batch[][2] = {
  {0.000f,  0.0f},
  {0.120f, 10.0f},
  {0.220f, 20.0f},
  {0.340f, 30.0f},
  {0.450f, 40.0f},
  {0.565f, 50.0f},
  {0.680f, 60.0f},
  {0.800f, 70.0f},
  {0.930f, 80.0f},
  {1.060f, 90.0f},
  {1.160f, 99.9f}
};

static const CalibrationTable CALIBRATION_TABLES[] = {
  {"first_batch",  cal_first_batch,  sizeof(cal_first_batch)  / sizeof(cal_first_batch[0])},
  {"second_batch", cal_second_batch, sizeof(cal_second_batch) / sizeof(cal_second_batch[0])},
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
