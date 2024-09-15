#include "camera.h"

static camera_t camera = {
    .position = {0, 2, 0},
    .direction = {0, 0, 1},
    .forward_velocity = {0, 0, 0},
    .pitch = 0.0f,
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

void change_camera_forward_velocity_to_z(float factor)
{
    camera.forward_velocity = vec3_mul(vec3_normalize(vec3_add(camera.forward_velocity, camera.direction)), factor);
}

void change_camera_forward_velocity_to_y(float factor)
{
    camera.forward_velocity = vec3_mul(vec3_normalize(vec3_add(camera.forward_velocity, CAMERA_UP)), factor);
}

void change_camera_forward_velocity_to_x(float factor)
{
    camera.forward_velocity = vec3_mul(vec3_normalize(vec3_add(camera.forward_velocity, vec3_cross(CAMERA_UP, camera.direction))), factor);
}

void move_camera_by_forward_velocity(void)
{
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