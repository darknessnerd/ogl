/*
  This header is based on 3DMath.h from DigiBen.
  Adapted by Julien Folly for the 3DSloader.h.

  Date : 11.12.04
*/

#include "3DS.h"

int Collision3DScount; // Initialisation d'un compteur pour les fonctions 3DS

/////////////////////////////////////// ABSOLUTE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the absolute value of the number passed in
/////
/////////////////////////////////////// ABSOLUTE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Absolute(float num)
{
	// If num is less than zero, we want to return the absolute value of num.
	// This is simple, either we times num by -1 or subtract it from 0.
	if(num < 0)
		return (0 - num);

	// Return the original number because it was already positive
	return num;
}

// Cette fonction calcul les coordonnées d'un point après une rotation
obj_vertex Rotate(obj_vertex tmp,float rx,float ry,float rz)
{
    float A,a,b;
                // Calcul d'une rotation X
                A=sqrt(tmp.z*tmp.z+tmp.y*tmp.y);
                b=atanf(Absolute(tmp.z)/Absolute(tmp.y))*57.295779; // 57.295779=360/(2*PI)
                if     (tmp.z<0 && tmp.y>0) a=360-b;
                else if(tmp.z>0 && tmp.y>0) a=b;
                else if(tmp.z>0 && tmp.y<0) a=180-b;
                else if(tmp.z<0 && tmp.y<0) a=180+b;
                tmp.y=A*cos((a+rx)*0.017453); // 0.017453=1/360*(2*PI)
                tmp.z=A*sin((a+rx)*0.017453);

                // Calcul d'une rotation Y
                A=sqrt(tmp.x*tmp.x+tmp.z*tmp.z);
                b=atan(Absolute(tmp.x)/Absolute(tmp.z))*57.295779;
                if     (tmp.x<0 && tmp.z>0) a=360-b;
                else if(tmp.x>0 && tmp.z>0) a=b;
                else if(tmp.x>0 && tmp.z<0) a=180-b;
                else if(tmp.x<0 && tmp.z<0) a=180+b;
                tmp.z=A*cos((a+ry)*0.017453);
                tmp.x=A*sin((a+ry)*0.017453);

                // Calcul d'une rotation Z
                A=sqrt(tmp.x*tmp.x+tmp.y*tmp.y);
                b=atan(Absolute(tmp.x)/Absolute(tmp.y))*57.295779;
                if     (tmp.x<0 && tmp.y>0) a=360-b;
                else if(tmp.x>0 && tmp.y>0) a=b;
                else if(tmp.x>0 && tmp.y<0) a=180-b;
                else if(tmp.x<0 && tmp.y<0) a=180+b;
                tmp.y=A*cos((a+rz)*0.017453);
                tmp.x=A*sin((a+rz)*0.017453);
   return tmp;
}

/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a perpendicular vector from 2 given vectors by taking the cross product.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

obj_vertex Cross(obj_vertex vVector1, obj_vertex vVector2)
{
	obj_vertex vNormal;									// The vector to hold the cross product

	// The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)													// Get the X value
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));

	// The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));

	// The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vNormal;										// Return the cross product (Direction the polygon is facing - Normal)
}

/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a normal (or any other vector)
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(obj_vertex vNormal)
{
	// This will give us the magnitude or "Norm" as some say, of our normal.
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)  Where V is the vector

	return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) );
}

/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

obj_vertex Normalize(obj_vertex vNormal)
{
	float magnitude = Magnitude(vNormal);				// Get the magnitude of our normal

	// Now that we have the magnitude, we can divide our normal by that magnitude.
	// That will make our normal a total length of 1.  This makes it easier to work with too.

	vNormal.x /= magnitude;								// Divide the X value of our normal by it's magnitude
	vNormal.y /= magnitude;								// Divide the Y value of our normal by it's magnitude
	vNormal.z /= magnitude;								// Divide the Z value of our normal by it's magnitude

	// Finally, return our normalized normal.

	return vNormal;										// Return the new normal of length 1.
}


/////////////////////////////////////// NORMAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the normal of a polygon (The direction the polygon is facing)
/////
/////////////////////////////////////// NORMAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

obj_vertex Normal(obj_vertex vPolygon[])
{														// Get 2 vectors from the polygon (2 sides), Remember the order!
	obj_vertex vVector1;
    vVector1.x = vPolygon[2].x - vPolygon[0].x;
	vVector1.y = vPolygon[2].y - vPolygon[0].y;
	vVector1.z = vPolygon[2].z - vPolygon[0].z;

	obj_vertex vVector2;
	vVector2.x = vPolygon[1].x - vPolygon[0].x;
	vVector2.y = vPolygon[1].y - vPolygon[0].y;
	vVector2.z = vPolygon[1].z - vPolygon[0].z;

	obj_vertex vNormal = Cross(vVector1, vVector2);		// Take the cross product of our 2 vectors to get a perpendicular vector

	// Now we have a normal, but it's at a strange length, so let's make it length 1.

	vNormal = Normalize(vNormal);						// Use our function we created to normalize the normal (Makes it a length of one)

	return vNormal;										// Return our normal at our desired length
}

/////////////////////////////////// PLANE DISTANCE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the distance between a plane and the origin
/////
/////////////////////////////////// PLANE DISTANCE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float PlaneDistance(obj_vertex Normal, obj_vertex Point)
{
	float distance = 0;									// This variable holds the distance from the plane tot he origin

	// Use the plane equation to find the distance (Ax + By + Cz + D = 0)  We want to find D.
	// So, we come up with D = -(Ax + By + Cz)
														// Basically, the negated dot product of the normal of the plane and the point. (More about the dot product in another tutorial)
	distance = - ((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));

	return distance;									// Return the distance
}

/////////////////////////////////// INTERSECTED PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This checks to see if a line intersects a plane
/////
/////////////////////////////////// INTERSECTED PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool IntersectedPlane(obj_vertex vPoly[], obj_vertex vLine[], obj_vertex &vNormal, float &originDistance)
{
	float distance1=0, distance2=0;						// The distances from the 2 points of the line from the plane

	vNormal = Normal(vPoly);							// We need to get the normal of our plane to go any further

	// Let's find the distance our plane is from the origin.  We can find this value
	// from the normal to the plane (polygon) and any point that lies on that plane (Any vertex)
	originDistance = PlaneDistance(vNormal, vPoly[0]);

	// Get the distance from point1 from the plane using: Ax + By + Cz + D = (The distance from the plane)

	distance1 = ((vNormal.x * vLine[0].x)  +					// Ax +
		         (vNormal.y * vLine[0].y)  +					// Bx +
				 (vNormal.z * vLine[0].z)) + originDistance;	// Cz + D

	// Get the distance from point2 from the plane using Ax + By + Cz + D = (The distance from the plane)

	distance2 = ((vNormal.x * vLine[1].x)  +					// Ax +
		         (vNormal.y * vLine[1].y)  +					// Bx +
				 (vNormal.z * vLine[1].z)) + originDistance;	// Cz + D

	// Now that we have 2 distances from the plane, if we times them together we either
	// get a positive or negative number.  If it's a negative number, that means we collided!
	// This is because the 2 points must be on either side of the plane (IE. -1 * 1 = -1).

	if(distance1 * distance2 >= 0)			// Check to see if both point's distances are both negative or both positive
	   return false;						// Return false if each point has the same sign.  -1 and 1 would mean each point is on either side of the plane.  -1 -2 or 3 4 wouldn't...

	return true;							// The line intersected the plane, Return TRUE
}

/////////////////////////////////// DOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This computers the dot product of 2 vectors
/////
/////////////////////////////////// DOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Dot(obj_vertex vVector1, obj_vertex vVector2)
{
	// The dot product is this equation: V1.V2 = (V1.x * V2.x  +  V1.y * V2.y  +  V1.z * V2.z)
	// In math terms, it looks like this:  V1.V2 = ||V1|| ||V2|| cos(theta)

			 //    (V1.x * V2.x        +        V1.y * V2.y        +        V1.z * V2.z)
	return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) );
}


/////////////////////////////////// ANGLE BETWEEN VECTORS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This checks to see if a point is inside the ranges of a polygon
/////
/////////////////////////////////// ANGLE BETWEEN VECTORS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

double AngleBetweenVectors(obj_vertex Vector1, obj_vertex Vector2)
{
	// Get the dot product of the vectors
	double dotProduct = Dot(Vector1, Vector2);

	// Get the product of both of the vectors magnitudes
	double vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ;

	// Get the angle in radians between the 2 vectors
	double angle = acos( dotProduct / vectorsMagnitude );

	// Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinate
	// I don't active the following command because it's don't run under linux
	//if(_isnan(angle))
	//	return 0;

	// Return the angle in radians
	return( angle );
}

/////////////////////////////////// INTERSECTION POINT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the intersection point of the line that intersects the plane
/////
/////////////////////////////////// INTERSECTION POINT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

obj_vertex IntersectionPoint(obj_vertex vNormal, obj_vertex vLine[], double distance)
{
	obj_vertex vPoint, vLineDir;					// Variables to hold the point and the line's direction
	double Numerator = 0.0, Denominator = 0.0, dist = 0.0;

	// 1)  First we need to get the vector of our line, Then normalize it so it's a length of 1
	vLineDir.x = vLine[1].x - vLine[0].x;
	vLineDir.y = vLine[1].y - vLine[0].y;
	vLineDir.z = vLine[1].z - vLine[0].z;			// Get the Vector of the line

    vLineDir = Normalize(vLineDir);				// Normalize the lines vector


	// 2) Use the plane equation (distance = Ax + By + Cz + D) to find the
	// distance from one of our points to the plane.
	Numerator = - (vNormal.x * vLine[0].x +		// Use the plane equation with the normal and the line
				   vNormal.y * vLine[0].y +
				   vNormal.z * vLine[0].z + distance);

	// 3) If we take the dot product between our line vector and the normal of the polygon,
	Denominator = Dot(vNormal, vLineDir);		// Get the dot product of the line's vector and the normal of the plane

	// Since we are using division, we need to make sure we don't get a divide by zero error
	// If we do get a 0, that means that there are INFINATE points because the the line is
	// on the plane (the normal is perpendicular to the line - (Normal.Vector = 0)).
	// In this case, we should just return any point on the line.

	if( Denominator == 0.0)						// Check so we don't divide by zero
		return vLine[0];						// Return an arbitrary point on the line

	dist = Numerator / Denominator;				// Divide to get the multiplying (percentage) factor

	// Now, like we said above, we times the dist by the vector, then add our arbitrary point.
	vPoint.x = (float)(vLine[0].x + (vLineDir.x * dist));
	vPoint.y = (float)(vLine[0].y + (vLineDir.y * dist));
	vPoint.z = (float)(vLine[0].z + (vLineDir.z * dist));

	return vPoint;								// Return the intersection point
}

/////////////////////////////////// INSIDE POLYGON \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This checks to see if a point is inside the ranges of a polygon
/////
/////////////////////////////////// INSIDE POLYGON \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool InsidePolygon(obj_vertex vIntersection, obj_vertex Poly[], long verticeCount)
{
	const double MATCH_FACTOR = 0.99;		// Used to cover up the error in floating point
	double Angle = 0.0;						// Initialize the angle
	obj_vertex vA, vB;						// Create temp vectors

	for (int i = 0; i < verticeCount; i++)		// Go in a circle to each vertex and get the angle between
	{
		vA.x = Poly[i].x - vIntersection.x;			// Subtract the intersection point from the current vertex
		vA.y = Poly[i].y - vIntersection.y;
        vA.z = Poly[i].z - vIntersection.z;
        										// Subtract the point from the next vertex
		vB.x = Poly[(i + 1) % verticeCount].x - vIntersection.x;
		vB.y = Poly[(i + 1) % verticeCount].y - vIntersection.y;
		vB.z = Poly[(i + 1) % verticeCount].z - vIntersection.z;

		Angle += AngleBetweenVectors(vA, vB);	// Find the angle between the 2 vectors and add them all up as we go along
	}

	if(Angle >= (MATCH_FACTOR * (2.0 * PI)) )	// If the angle is greater than 2 PI, (360 degrees)
		return true;							// The point is inside of the polygon

	return false;								// If you get here, it obviously wasn't inside the polygon, so Return FALSE
}

/////////////////////////////////// INTERSECTED POLYGON \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This checks if a line is intersecting a polygon
/////
/////////////////////////////////// INTERSECTED POLYGON \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool IntersectedPolygon(obj_vertex vPoly[], obj_vertex vLine[], int verticeCount)
{
	obj_vertex vNormal;
	float originDistance = 0;

	// First, make sure our line intersects the plane
									 // Reference   // Reference
	if(!IntersectedPlane(vPoly, vLine,   vNormal,   originDistance))
		return false;

	// Now that we have our normal and distance passed back from IntersectedPlane(),
	// we can use it to calculate the intersection point.
	obj_vertex vIntersection = IntersectionPoint(vNormal, vLine, originDistance);

	// Now that we have the intersection point, we need to test if it's inside the polygon.
	if(InsidePolygon(vIntersection, vPoly, verticeCount))
		return true;							// We collided!	  Return success

	return false;								// There was no collision, so return false
}

// Test si une ligne traverse un triangle d'un object ou d'une scène 3DS
// La syntax de cette fonction est semblable à celle Collision3DS()
bool Intersected3DS(obj_vertex vLine[],obj_type& object,int obj,float ox,float oy,float oz,float rx,float ry,float rz,float ag,int step,bool vari){
  /*  int tobj,init;
    obj_vertex tmp[3];
    rz=360-rz;
    if(obj==-1)
    {
        tobj=object.nbobj;
        obj=0;
    }
    else tobj=obj;

    // Petit procéder pour varier les triangles testés sur l'objet
    if(vari){
        Collision3DScount++;
        if(Collision3DScount>=step) Collision3DScount=0;
        init=Collision3DScount;
    }else init=0;

    for(obj;obj<=tobj;++obj){
        for(int l_index=init;l_index<object.nobj[obj].polygons_qty;l_index+=step){
                // Coordonnées du premier vertex
                tmp[0].x=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].a ].x*ag;
                tmp[0].y=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].a ].y*ag;
                tmp[0].z=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].a ].z*ag;

                // Calcul les rotations
                tmp[0]=Rotate(tmp[0],rx,ry,rz);

                // Ajoute la translation
                tmp[0].x+=ox;
                tmp[0].y+=oy;
                tmp[0].z+=oz;

                // Coordonnées du second vertex
                tmp[1].x=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].b ].x*ag;
                tmp[1].y=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].b ].y*ag;
                tmp[1].z=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].b ].z*ag;

                // Calcul les rotations
                tmp[1]=Rotate(tmp[1],rx,ry,rz);

                // Ajoute la translation
                tmp[1].x+=ox;
                tmp[1].y+=oy;
                tmp[1].z+=oz;

                // Coordonnées du troisième vertex
                tmp[2].x=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].c ].x*ag;
                tmp[2].y=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].c ].y*ag;
                tmp[2].z=object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].c ].z*ag;

                // Calcul les rotations
                tmp[2]=Rotate(tmp[2],rx,ry,rz);

                // Ajoute la translation
                tmp[2].x+=ox;
                tmp[2].y+=oy;
                tmp[2].z+=oz;

                if(IntersectedPolygon(tmp,vLine,3)) return 1;
        }
    }    */
return 0;
}

// Test si deux objets ou deux scène 3DS rentrent en collision
//
// Syntax :
//  Collision3DS(
//      nom du premier obj_type où est enregistrée la scène 3DS,
//      numéro des objets à traité de la première scène (-1 = traite tous les objets)
//      translation x effectué sur le premier object 3DS,
//      translation y effectué,
//      translation z effectué,
//      rotation x effectué sur le premier object 3DS,
//      rotation y effectué,
//      rotation z effectué,
//      agrandissement effectué sur le premier object 3DS (laisser 1 si default)
//      nombre de polygone à traité (1 = tous les polygone mais c'est très lent, 2 = 1 poly sur 2, etc.)
//
//      nom du deuxième obj_type où est enregistrée la scène 3DS,
//      numéro des objets à traité de la deuxième scène (-1 = traite tous les objets)
//      translation x effectué sur le deuxième object 3DS,
//      translation y effectué,
//      translation z effectué,
//      rotation x effectué sur le deuxième object 3DS,
//      rotation y effectué,
//      rotation z effectué,
//      agrandissement effectué sur le deuxième object 3DS (laisser 1 si default)
//      nombre de polygone à traité (1 = tous les polygone mais c'est très lent, 2 = 1 poly sur 2, etc.)
//      vari 1 = Change de polygones testés, 0 = ne change pas (inutile de mettre 1 si step=0, effet sur les 2 objets)
//      )
bool Collision3DS(obj_type& object0,int obj0,float ox0,float oy0,float oz0,float rx0,float ry0,float rz0,float ag0,int step0,
                  obj_type& object1,int obj1,float ox1,float oy1,float oz1,float rx1,float ry1,float rz1,float ag1,int step1,bool vari){
   /* int tobj,init;
    obj_vertex vLine[2];
    rz0=360-rz0;
    if(obj0==-1)
    {
        tobj=object0.nbobj;
        obj0=0;
    }
    else tobj=obj0;

    // Petit procéder pour varier les triangles testés sur l'"object0"
    if(vari){
        Collision3DScount++;
        if(Collision3DScount>=step0) Collision3DScount=0;
        init=Collision3DScount;
    }else init=0;

    // Boucle pour tester les triangles
    for(obj0;obj0<=tobj;++obj0){
        for(int l_index=init;l_index<object0.nobj[obj0].polygons_qty;l_index+=step0){
                // Coordonnées du premier vertex
                vLine[0].x=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].a ].x*ag0;
                vLine[0].y=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].a ].y*ag0;
                vLine[0].z=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].a ].z*ag0;

                // Calcul les rotations
                vLine[0]=Rotate(vLine[0],rx0,ry0,rz0);

                // Ajoute la translation
                vLine[0].x+=ox0;
                vLine[0].y+=oy0;
                vLine[0].z+=oz0;

                // Coordonnées du second vertex
                vLine[1].x=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].b ].x*ag0;
                vLine[1].y=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].b ].y*ag0;
                vLine[1].z=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].b ].z*ag0;

                // Calcul les rotations
                vLine[1]=Rotate(vLine[1],rx0,ry0,rz0);

                // Ajoute la translation
                vLine[1].x+=ox0;
                vLine[1].y+=oy0;
                vLine[1].z+=oz0;

                if(Intersected3DS(vLine,object1,obj1,ox1,oy1,oz1,rx1,ry1,rz1,ag1,step1,vari)) return 1;

                // Coordonnées du second vertex
                vLine[0].x=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].b ].x*ag0;
                vLine[0].y=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].b ].y*ag0;
                vLine[0].z=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].b ].z*ag0;

                // Calcul les rotations
                vLine[0]=Rotate(vLine[0],rx0,ry0,rz0);

                // Ajoute la translation
                vLine[0].x+=ox0;
                vLine[0].y+=oy0;
                vLine[0].z+=oz0;

                // Coordonnées du troisième vertex
                vLine[1].x=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].c ].x*ag0;
                vLine[1].y=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].c ].y*ag0;
                vLine[1].z=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].c ].z*ag0;

                // Calcul les rotations
                vLine[1]=Rotate(vLine[1],rx0,ry0,rz0);

                // Ajoute la translation
                vLine[1].x+=ox0;
                vLine[1].y+=oy0;
                vLine[1].z+=oz0;

                if(Intersected3DS(vLine,object1,obj1,ox1,oy1,oz1,rx1,ry1,rz1,ag1,step1,vari)) return 1;

                // Coordonnées du troisième vertex
                vLine[0].x=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].c ].x*ag0;
                vLine[0].y=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].c ].y*ag0;
                vLine[0].z=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].c ].z*ag0;

                // Calcul les rotations
                vLine[0]=Rotate(vLine[0],rx0,ry0,rz0);

                // Ajoute la translation
                vLine[0].x+=ox0;
                vLine[0].y+=oy0;
                vLine[0].z+=oz0;

                // Coordonnées du premier vertex
                vLine[1].x=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].a ].x*ag0;
                vLine[1].y=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].a ].y*ag0;
                vLine[1].z=object0.nobj[obj0].vertex[ object0.nobj[obj0].polygon[l_index].a ].z*ag0;

                // Calcul les rotations
                vLine[1]=Rotate(vLine[1],rx0,ry0,rz0);

                // Ajoute la translation
                vLine[1].x+=ox0;
                vLine[1].y+=oy0;
                vLine[1].z+=oz0;

                if(Intersected3DS(vLine,object1,obj1,ox1,oy1,oz1,rx1,ry1,rz1,ag1,step1,vari)) return 1;
        }
    }*/
   return 0;
}
