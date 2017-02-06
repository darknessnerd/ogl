//!  3d base object.
/*!
  Base 3d object.
*/
#ifndef BASEOBJECT_H
#define BASEOBJECT_H
#include <GL/glew.h> // include GLEW and new version of GL on Windows
namespace core
{
namespace gl
{

class BaseObject
{
    public:
        BaseObject(const GLfloat vp[], GLuint vp_size, const GLfloat vc[], GLuint vc_size, const GLfloat vn[], GLuint vn_size, const GLfloat vt[], GLuint vt_size);
        virtual ~BaseObject();

        GLuint getVao() const;

    protected:

    private:

       GLfloat* vp = NULL; // array of vertex points
       GLfloat* vc = NULL; // array of colour points
	   GLfloat* vn = NULL; // array of vertex normals
       GLfloat* vt = NULL; // array of texture coordinates

       GLuint vp_size;
       GLuint vc_size;
       GLuint vt_size;
       GLuint vn_size;

       //vertex buffer objects
       GLuint vp_vbo;
       GLuint vc_vbo;
       GLuint vt_vbo;
       GLuint vn_vbo;

       //vertex attribute object
       GLuint vao;
};

}
}

#endif // BASEOBJECT_H
