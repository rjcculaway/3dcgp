#include "camera.h"

camera_t camera = {
    .position = {0, 0, 0},
    .direction = {0, 0, 1},
    .forward_velocity = {0, 0, 0},
    .yaw = 0.0f};

// Up vector for the view matrix
const vec3_t CAMERA_UP = {0, 1, 0};