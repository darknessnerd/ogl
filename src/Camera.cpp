#include "Camera.h"
using namespace core::gl;


Camera::Camera(const float x, const float y, const float z, const float near, const float far, const float fovy, const float window_width, const float window_height):near(near), far(far), fovy(fovy)
{
    this->aspect = (float)window_width / (float)window_height; // aspect ratio

    //initial cam position
    this->position.v[0] = x;
    this->position.v[1] = y;
    this->position.v[2] = z;
    //#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
    this->fovy = fovy*0.017444444;
    projM = perspective (fovy, aspect, near, far);

    //camera setup
    this->speed = 5.0f; // 1 unit per second
    this->heading_speed = 10.0f; // 30 degrees per second
    this->heading = 0.0f; // y-rotation in degrees
    this->T = translate (
                  identity_mat4 (), vec3 (-position.v[0], -position.v[1], -position.v[2])
              );
    this->R = rotate_y_deg (identity_mat4 (), -this->heading);
    this->q = quat_from_axis_deg (-this->heading, 0.0f, 1.0f, 0.0f);
    this->viewM = R * T;
    // keep track of some useful vectors that can be used for keyboard movement
    this->fwd.v[0] = this->fwd.v[1] = this->fwd.v[3] = 0.0f;
    this->fwd.v[2] = -1.0f;

    this->rgt.v[1] = this->fwd.v[2] = this->fwd.v[3] = 0.0f;
    this->rgt.v[0] = 1.0f;

    this->rgt.v[0] = this->fwd.v[2] = this->fwd.v[3] = 0.0f;
    this->rgt.v[1] = 1.0f;

    this->reset();
}

bool Camera::isMoved()
{
    if (moved)
    {
        // re-calculate local axes so can move fwd in dir cam is pointing
        R = quat_to_mat4 (q);
        fwd = R * vec4 (0.0, 0.0, -1.0, 0.0);
        rgt = R * vec4 (1.0, 0.0, 0.0, 0.0);
        up = R * vec4 (0.0, 1.0, 0.0, 0.0);

        position = position + vec3 (fwd) * -move.v[2];
        position = position + vec3 (up) * move.v[1];
        position = position + vec3 (rgt) * move.v[0];
        mat4 T = translate (identity_mat4 (), vec3 (position));

        viewM = inverse (R) * inverse (T);


        this->reset();
        return true;
    }
    return false;
}


void Camera::roll(Direction direction, float elapsed_seconds)
{

    int sign = 1;
    if(direction == Camera::Direction::minus)
        sign = -1;
    roll_degree += sign * heading_speed * elapsed_seconds;
    moved = true;
    versor q_roll = quat_from_axis_deg (
                        roll_degree, fwd.v[0], fwd.v[1], fwd.v[2]
                    );
    q = q_roll * q;

}
void Camera::yaw(Direction direction, float elapsed_seconds)
{
 int sign = 1;
    if(direction == Camera::Direction::minus)
        sign = -1;
      yaw_degree -= sign * heading_speed * elapsed_seconds;
            moved = true;
            versor q_yaw = quat_from_axis_deg (
                               yaw_degree, up.v[0], up.v[1], up.v[2]
                           );
            q = q_yaw * q;
}
void Camera::pitch(Direction direction, float elapsed_seconds)
{
    int sign = 1;
    if(direction == Camera::Direction::minus)
        sign = -1;

    pitch_degree -= sign * heading_speed * elapsed_seconds;
    moved = true;
    versor q_pitch = quat_from_axis_deg (
                         pitch_degree, rgt.v[0], rgt.v[1], rgt.v[2]
                     );
    q = q_pitch * q;


}

void Camera::traslation(Axis axis, Direction direction, float elapsed_seconds)
{
    int sign = 1;
    if(direction == Camera::Direction::minus)
        sign = -1;
    moved = true;
    switch(axis)
    {
       case Camera::Axis::x:
          move.v[0] += sign * speed * elapsed_seconds;
          break;
       case Camera::Axis::y:
          move.v[1] += sign * speed * elapsed_seconds;
          break;
       case Camera::Axis::z:
          move.v[2] += sign * speed * elapsed_seconds;
          break;
    }
}
void Camera::reset()
{
    //util variable camera movment
    this->moved = false;
    this->move.v[0] = this->move.v[1] = this->move.v[2] = this->move.v[3] = 0.0f;
    this->yaw_degree = 0.0f; // y-rotation in degrees
    this->pitch_degree = 0.0f;
    this->roll_degree = 0.0;
}


const mat4 Camera::getProj() const
{
    return this->projM;
}

const mat4 Camera::getView() const
{
    return this->viewM;
}

Camera::~Camera()
{
    //dtor
}
