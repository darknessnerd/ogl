#ifndef CAMERA_H
#define CAMERA_H
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
#include "../maths_funcs.h"
namespace core
{
namespace gl
{
class Camera
{
    public:
        Camera(const float x, const float y, const float z, const float near, const float far, const float fovy, const float window_width, const float window_height);
        virtual ~Camera();


        const mat4 getView() const ;
        const mat4 getProj() const ;

        bool isMoved();

        enum Axis {x,y,z};
        enum Direction {minus, plus};

        void traslation(Axis axis, Direction direction, float elapsed_seconds);
        void roll(Direction direction, float elapsed_seconds);
        void yaw(Direction direction, float elapsed_seconds);
        void pitch(Direction direction, float elapsed_seconds);

    protected:


    private:
       //clipping plane
       float near;
       float far;
       float fovy;

       float aspect;

       //camera values
       float speed;
       float heading_speed;
       float heading;

       mat4 viewM;
       mat4 projM;

       mat4 T;
       mat4 R;

       vec4 fwd;
       vec4 rgt;
       vec4 up;

       versor q;

       vec3 position;

       //util variable camera movment
       bool moved;
       vec3 move;
       float yaw_degree; // y-rotation in degrees
       float pitch_degree;
       float roll_degree;

       void reset();
};
}
}





#endif // CAMERA_H
