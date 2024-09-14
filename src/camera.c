#include "camera.h"

static camera_t camera = {
    .position = {0, 0, 0},
    .direction = {0, 0, 1},
    .forward_velocity = {0, 0, 0},
    .yaw = 0.0f};

// Up vector for the view matrix
const vec3_t CAMERA_UP = {0, 1, 0};

vec3_t get_camera_position(void)
{
    return camera.position;
}
vec3_t get_camera_direction(void)
{
    return camera.direction;
}
vec3_t get_camera_forward_velocity(void)
{
    return camera.forward_velocity;
}
float get_camera_yaw(void)
{
    return camera.yaw;
}
float get_camera_pitch(void)
{
    return camera.pitch;
}

void set_camera_position(vec3_t position)
{
    camera.position = position;
}
void set_camera_direction(vec3_t direction)
{
    camera.direction = direction;
}
void set_camera_forward_velocity(vec3_t forward_velocity)
{
    camera.forward_velocity = forward_velocity;
}
void set_camera_yaw(float yaw)
{
    camera.yaw = yaw;
}
void set_camera_pitch(float pitch)
{
    camera.pitch = pitch;
}

void move_camera_z(float factor)
{
    camera.forward_velocity = vec3_mul(camera.direction, factor);
    camera.position = vec3_add(camera.position, camera.forward_velocity);
}

void move_camera_y(float factor)
{
    camera.forward_velocity = vec3_mul(vec3_create(0, 1, 0), factor);
    camera.position = vec3_add(camera.position, camera.forward_velocity);
}

void move_camera_x(float factor)
{
    camera.forward_velocity = vec3_mul(vec3_cross(CAMERA_UP, camera.direction), factor);
    camera.position = vec3_add(camera.position, camera.forward_velocity);
}

void rotate_camera_yaw(float angle)
{
    camera.yaw += angle;
}
void rotate_camera_pitch(float angle)
{
    camera.pitch += angle;
}

vec3_t get_camera_target()
{
    return vec3_add(camera.position, camera.direction);
}