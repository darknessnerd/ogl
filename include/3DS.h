// Pour plus d'information voir 3dsloader.cpp et 3DSCollision.cpp

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <GL/glut.h>
#include <math.h>	// pour les fontions atanf() sqrt() cos() sin()

#define PI 3.1415926535897932					// la valeur PI
#define MAX_VERTICES 2000 // Maximun de vertex par objet
#define MAX_POLYGONS 2000 // Maximum de polygones par object
#define MAX_OBJ 20 // Maximum d'objets

// Strucure Vertex
typedef struct{
    float x,y,z;
}obj_vertex;

// Le polygone (triangle)
typedef struct{
    int a,b,c;
}polygon_type;

// La coordination des texture pour chaque vertex
typedef struct{
    float u,v;
}mapcoord_type;

typedef struct{
    int vertices_qty;
    int polygons_qty;
    char name[20];
    float vertex[MAX_VERTICES];
    polygon_type polygon[MAX_POLYGONS];
    mapcoord_type mapcoord[MAX_VERTICES];
}nobj_type;

// Le type d'object
typedef struct {
    int nbobj;
    nobj_type nobj[MAX_OBJ];
    int id_texture;
} obj_type, *obj_type_ptr;

// Loader
void draw3DS(obj_type& object,int obj,float ag);
long filelength(int f);
char Load3DS (obj_type_ptr p_object, char *p_filename);

// Collision
float Absolute(float num);
obj_vertex Rotate(obj_vertex tmp,float rx,float ry,float rz);
obj_vertex Cross(obj_vertex vVector1, obj_vertex vVector2);
float Magnitude(obj_vertex vNormal);
obj_vertex Normalize(obj_vertex vNormal);
obj_vertex Normal(obj_vertex vPolygon[]);
float PlaneDistance(obj_vertex Normal, obj_vertex Point);
bool IntersectedPlane(obj_vertex vPoly[], obj_vertex vLine[], obj_vertex &vNormal, float &originDistance);
float Dot(obj_vertex vVector1, obj_vertex vVector2);
double AngleBetweenVectors(obj_vertex Vector1, obj_vertex Vector2);
obj_vertex IntersectionPoint(obj_vertex vNormal, obj_vertex vLine[], double distance);
bool InsidePolygon(obj_vertex vIntersection, obj_vertex Poly[], long verticeCount);
bool IntersectedPolygon(obj_vertex vPoly[], obj_vertex vLine[], int verticeCount);
bool Intersected3DS(obj_vertex vLine[],obj_type& object,int obj,float ox,float oy,float oz,float rx,float ry,float rz,float ag,int step,bool vari);
bool Collision3DS(obj_type& object0,int obj0,float ox0,float oy0,float oz0,float rx0,float ry0,float rz0,float ag0,int step0,
                  obj_type& object1,int obj1,float ox1,float oy1,float oz1,float rx1,float ry1,float rz1,float ag1,int step1,bool vari);


