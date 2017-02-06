/*
  Author: Damiano Vitulli <info@spacesimulator.net>
  Linux port : Panteleakis Ioannis <pioann@csd.auth.gr>
  Fonction draw3ds(obj_type object, int obj, int ag) and MultiObject: Julien Folly
*/

#include "3DS.h"

/**********************************************************

    FUNCTION draw3DS (obj_type, float)

    Cette fonction dessine un objet précédemment charger
    avec la fonction Load3DS (obj_type_ptr, char *)

    le deuxième argument spécifie quel object de la scène, il
    faut dessiner (-1 = dessine tous les objets)

    Le troisième argument est utile pour agrandir ou
    rapetissir l'object (1 = taille initiale)

 *********************************************************/

void draw3DS(obj_type& object,int obj,float ag){
    /*int l_index,tobj;
    if(obj==-1)
    {
        tobj=object.nbobj;
        obj=0;
    }
    else tobj=obj;
    glBegin(GL_TRIANGLES); // Commence à dessiner
    for(obj;obj<=tobj;++obj) {
        for (l_index=0;l_index<object.nobj[obj].polygons_qty;l_index++) {
                // Coordination texture pour le premier vertex
                glTexCoord2f( object.nobj[obj].mapcoord[ object.nobj[obj].polygon[l_index].a ].u,
                              object.nobj[obj].mapcoord[ object.nobj[obj].polygon[l_index].a ].v);
                // Coordonnées du premier vertex
                glVertex3f( object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].a ].x*ag,
                            object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].a ].y*ag,
                            object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].a ].z*ag);
                // Coordination texture pour le second vertex
                glTexCoord2f( object.nobj[obj].mapcoord[ object.nobj[obj].polygon[l_index].b ].u,
                              object.nobj[obj].mapcoord[ object.nobj[obj].polygon[l_index].b ].v);
                // Coordonnées du second vertex
                glVertex3f( object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].b ].x*ag,
                            object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].b ].y*ag,
                            object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].b ].z*ag);
                // Coordination texture pour le troisième vertex
                glTexCoord2f( object.nobj[obj].mapcoord[ object.nobj[obj].polygon[l_index].c ].u,
                              object.nobj[obj].mapcoord[ object.nobj[obj].polygon[l_index].c ].v);
                // Coordination texture pour le troisième vertex
                glVertex3f( object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].c ].x*ag,
                            object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].c ].y*ag,
                            object.nobj[obj].vertex[ object.nobj[obj].polygon[l_index].c ].z*ag);
        }
    }
    glEnd(); // Termine de dessiner
    */
}

long filelength(int f)
{
    struct stat buf;
    fstat(f, &buf);
    return(buf.st_size);
}

/**********************************************************

    FUNCTION Load3DS (obj_type_ptr, char *)

    Cette fonction charge les objects d'un fichiers 3ds.
    Cette fonction ne charge que les vertex, polygones,
    mapping lists de chaque objet.

 *********************************************************/

char Load3DS (obj_type_ptr p_object, char *p_filename)
{
	int i; //Index variable
	int obj=-1;
	FILE *l_file; //File pointer

	unsigned short l_chunk_id; //Chunk identifier
	unsigned int l_chunk_lenght; //Chunk lenght

	unsigned char l_char; //Char variable
	unsigned short l_qty; //Number of elements in each chunk

	unsigned short l_face_flags; //Flag that stores some face information

	//Open the file
	if ((l_file=fopen (p_filename, "rb"))== NULL)
	{
	printf("Unable to find : %s\n",p_filename);
	return 1; //Open the file
	}

	printf("Load 3ds file : %s\n",p_filename);

	while (ftell (l_file) < filelength (fileno (l_file))) //Loop to scan the whole file
	//while(!EOF)
	{
		//getch(); //Insert this command for debug (to wait for keypress for each chuck reading)

		fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
		//printf("ChunkID: %x\n",l_chunk_id);
		fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
		//printf("ChunkLenght: %x\n",l_chunk_lenght);

		switch (l_chunk_id)
        {
			//----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4d4d:
			break;

			//----------------- EDIT3DS -----------------
			// Description: 3D Editor chunk, objects layout info
			// Chunk ID: 3d3d (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x3d3d:
			break;

			//--------------- EDIT_OBJECT ---------------
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Lenght: len(object name) + sub chunks
			//-------------------------------------------
			case 0x4000:
				i=0;
				++obj;
				printf("  object no : %i\n",obj);
				p_object->nbobj=obj;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    p_object->nobj[obj].name[i]=l_char;
					i++;
                }while(l_char != '\0' && i<20);
			break;

			//--------------- OBJ_TRIMESH ---------------
			// Description: Triangular mesh, contains chunks for 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4100:
			break;

			//--------------- TRI_VERTEXL ---------------
			// Description: Vertices list
			// Chunk ID: 4110 (hex)
			// Chunk Lenght: 1 x unsigned short (number of vertices)
			//             + 3 x float (vertex coordinates) x (number of vertices)
			//             + sub chunks
			//-------------------------------------------
			case 0x4110:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                p_object->nobj[obj].vertices_qty = l_qty;
                printf("   Number of vertices: %d\n",l_qty);
                for (i=0; i<l_qty*3; i++)
                {
					fread (&p_object->nobj[obj].vertex[i], sizeof(float), 1, l_file);
 					//printf("Vertices list x: %f\n",p_object->vertex[i].x);
                    fread (&p_object->nobj[obj].vertex[++i], sizeof(float), 1, l_file);
 					//printf("Vertices list y: %f\n",p_object->vertex[i].y);
					fread (&p_object->nobj[obj].vertex[++i], sizeof(float), 1, l_file);
 					//printf("Vertices list z: %f\n",p_object->vertex[i].z);
				}
				break;

			//--------------- TRI_FACEL1 ----------------
			// Description: Polygons (faces) list
			// Chunk ID: 4120 (hex)
			// Chunk Lenght: 1 x unsigned short (number of polygons)
			//             + 3 x unsigned short (polygon points) x (number of polygons)
			//             + sub chunks
			//-------------------------------------------
			case 0x4120:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                p_object->nobj[obj].polygons_qty = l_qty;
                printf("   Number of polygons: %d\n",l_qty);
                for (i=0; i<l_qty; i++)
                {
					fread (&p_object->nobj[obj].polygon[i].a, sizeof (unsigned short), 1, l_file);
					//printf("Polygon point a: %d\n",p_object->polygon[i].a);
					fread (&p_object->nobj[obj].polygon[i].b, sizeof (unsigned short), 1, l_file);
					//printf("Polygon point b: %d\n",p_object->polygon[i].b);
					fread (&p_object->nobj[obj].polygon[i].c, sizeof (unsigned short), 1, l_file);
					//printf("Polygon point c: %d\n",p_object->polygon[i].c);
					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
					//printf("Face flags: %x\n",l_face_flags);
				}
                break;

			//------------- TRI_MAPPINGCOORS ------------
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Lenght: 1 x unsigned short (number of mapping points)
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub chunks
			//-------------------------------------------
			case 0x4140:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				for (i=0; i<l_qty; i++)
				{
					fread (&p_object->nobj[obj].mapcoord[i].u, sizeof (float), 1, l_file);
					//printf("Mapping list u: %f\n",p_object->mapcoord[i].u);
                    fread (&p_object->nobj[obj].mapcoord[i].v, sizeof (float), 1, l_file);
					//printf("Mapping list v: %f\n",p_object->mapcoord[i].v);
				}
                break;

			//----------- Skip unknow chunks ------------
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-------------------------------------------
			default:
				 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        }
	}
	fclose (l_file); // Closes the file stream
	return (0); // Returns ok
}
