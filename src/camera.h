#ifndef CAMERA_RENENGINE_SFW
#define CAMERA_RENENGINE_SFW

#include <stdio.h>
#include "vector.h"
#include "matrix.h"

typedef struct camera
{
  vec3_t position;
  vec3_t direction;
  vec3_t forward_velocity;
  float yaw;   // rotation around y
  float pitch; // rotation around x
} camera_t;

extern const vec3_t CAMERA_UP;

vec3_t get_camera_position(void);
vec3_t get_camera_direction(void);
vec3_t get_camera_forward_velocity(void);
float get_camera_yaw(void);
float get_camera_pitch(void);

void set_camera_position(vec3_t position);
void set_camera_direction(vec3_t direction);
void set_camera_forward_velocity(vec3_t forward_velocity);
void set_camera_yaw(float yaw);
void set_camera_pitch(float pitch);

void change_camera_forward_velocity_to_z(float factor);
void change_camera_forward_velocity_to_y(float factor);
void change_camera_forward_velocity_to_x(float factor);
void move_camera_by_forward_velocity(void);
void rotate_camera_yaw(float angle);
void rotate_camera_pitch(float angle);

vec3_t get_camera_target();
#endif
