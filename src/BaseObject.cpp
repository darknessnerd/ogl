#include "BaseObject.h"
using namespace core::gl;


BaseObject::BaseObject(const GLfloat vp[], GLuint vp_size, const GLfloat vc[], GLuint vc_size, const GLfloat vn[], GLuint vn_size, const GLfloat vt[], GLuint vt_size):vp_size(vp_size), vc_size(vc_size), vn_size(vn_size), vt_size(vt_size)
{

    //VERTEX BUFFER OBJECTS SETUP
    //POINTS
    this->vp = new GLfloat[vp_size];
    for(GLuint i = 0; i < vp_size; ++i)
       this->vp[i] = vp[i];

    //POINTS
    this->vc = new GLfloat[vc_size];
    for(GLuint i = 0; i < vc_size; ++i)
       this->vc[i] = vc[i];


    //normals
    this->vn = new GLfloat[vn_size];
    for(GLuint i = 0; i < vn_size; ++i)
       this->vn[i] = vn[i];

    //textures
    this->vt = new GLfloat[vt_size];
    for(GLuint i = 0; i < vt_size; ++i)
       this->vt[i] = vt[i];


    //setup the vertex buffer objects
    //VERTEX
    this->vp_vbo = 0;
    glGenBuffers(1, &vp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vp_vbo);
    //take the size in bytes of the array
    glBufferData(GL_ARRAY_BUFFER, this->vp_size*sizeof(GLfloat), this->vp, GL_STATIC_DRAW);

    //colours
    this->vc_vbo = 0;
    glGenBuffers(1, &vc_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vc_vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vc_size*sizeof(GLfloat), this->vc, GL_STATIC_DRAW);



    //normals
    this->vn_vbo = 0;
    glGenBuffers(1, &vn_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vn_vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vn_size*sizeof(GLfloat), this->vn, GL_STATIC_DRAW);

     //textures
    this->vt_vbo = 0;
    glGenBuffers(1, &vt_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vt_vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vt_size*sizeof(GLfloat), this->vt, GL_STATIC_DRAW);



    //final object - vertex attribute object
    this->vao = 0;
    glGenVertexArrays(1, &this->vao);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vp_vbo);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);


    glBindBuffer(GL_ARRAY_BUFFER, this->vn_vbo);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 0, NULL);



    glBindBuffer(GL_ARRAY_BUFFER, this->vc_vbo);
    glVertexAttribPointer(2,3,GL_FLOAT, GL_FALSE, 0, NULL);


    glBindBuffer(GL_ARRAY_BUFFER, this->vt_vbo);
    glVertexAttribPointer(3,2,GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);


}

 GLuint BaseObject::getVao() const
 {
    return this->vao;
 }

BaseObject::~BaseObject()
{
    delete vp;
    delete vt;
    delete vn;
    delete vc;
}
