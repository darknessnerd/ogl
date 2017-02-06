#include "GLFWWindow.h"
#include "BaseObject.h"
#include "LeapListener.h"
using namespace core::utils::leap;
#include "Camera.h"
#include "3DS.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define VERTEX_SHADER_FILE "test_vs.glsl"
#define FRAGMENT_SHADER_FILE "test_fs.glsl"
#define MAX_SHADER_LENGTH 262144
void print_shader_programme_info_log (GLuint sp) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog (sp, max_length, &actual_length, log);
	printf ("program info log for GL index %u:\n%s", sp, log);
}
bool is_shader_programme_valid (GLuint sp) {
	glValidateProgram (sp);
	GLint params = -1;
	glGetProgramiv (sp, GL_VALIDATE_STATUS, &params);
	if (GL_TRUE != params) {

		print_shader_programme_info_log (sp);
		return false;
	}
	return true;
}

bool create_programme_gl (GLuint vert, GLuint frag, GLuint* programme) {
	*programme = glCreateProgram ();
	/*gl_log (
		"created programme %u. attaching shaders %u and %u...\n",
		*programme,
		vert,
		frag
	);*/
	glAttachShader (*programme, vert);
	glAttachShader (*programme, frag);
	// link the shader programme. if binding input attributes do that before link

	glBindAttribLocation(*programme, 0, "vertex_position");
    glBindAttribLocation(*programme, 1, "vertex_normal");
    glBindAttribLocation(*programme, 2, "vertex_colour");
    glBindAttribLocation(*programme, 3, "vertex_texture");

	glLinkProgram (*programme);
	GLint params = -1;
	glGetProgramiv (*programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		printf (
			"ERROR: could not link shader programme GL index %u\n",
			*programme
		);
		//print_programme_info_log (*programme);
		return false;
	}
	is_shader_programme_valid (*programme);
	// delete shaders here to free memory
	glDeleteShader (vert);
	glDeleteShader (frag);
	return true;
}


/*-----------------------------------SHADERS----------------------------------*/
bool parse_file_into_str_gl (
	const char* file_name, char* shader_str, int max_len
) {
	shader_str[0] = '\0'; // reset string
	FILE* file = fopen (file_name , "r");
	if (!file) {
		//gl_log_err ("ERROR: opening file for reading: %s\n", file_name);
		return false;
	}
	int current_len = 0;
	char line[2048];
	strcpy (line, ""); // remember to clean up before using for first time!
	while (!feof (file)) {
		if (NULL != fgets (line, 2048, file)) {
			current_len += strlen (line); // +1 for \n at end
			if (current_len >= max_len) {
				//gl_log_err (
					//"ERROR: shader length is longer than string buffer length %i\n",
					//max_len
				//);
			}
			strcat (shader_str, line);
		}
	}
	if (EOF == fclose (file)) { // probably unnecesssary validation
//		gl_log_err ("ERROR: closing file from reading %s\n", file_name);
		return false;
	}
	return true;
}
bool create_shader_gl (const char* file_name, GLuint* shader, GLenum type) {
	//gl_log ("creating shader from %s...\n", file_name);
	char shader_string[MAX_SHADER_LENGTH];
	parse_file_into_str_gl (file_name, shader_string, MAX_SHADER_LENGTH);
	*shader = glCreateShader (type);
	const GLchar* p = (const GLchar*)shader_string;
	glShaderSource (*shader, 1, &p, NULL);
	glCompileShader (*shader);
	// check for compile errors
	int params = -1;
	glGetShaderiv (*shader, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		printf ("ERROR: GL shader index %i did not compile\n", *shader);
		//print_shader_info_log (*shader);
		return false; // or exit or something
	}
	//gl_log ("shader compiled. index %i\n", *shader);
	return true;
}
GLuint create_programme_from_files_gl (
	const char* vert_file_name, const char* frag_file_name
) {
	GLuint vert, frag, programme;
	printf("vertex\n");
	create_shader_gl (vert_file_name, &vert, GL_VERTEX_SHADER);
	printf("fragment\n");
	create_shader_gl (frag_file_name, &frag, GL_FRAGMENT_SHADER);
	create_programme_gl (vert, frag, &programme);
	return programme;
}


bool load_texture (const char* file_name, GLuint* tex, GLuint shader_programme ) {


	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	printf("image size  : %d %d\n", x, y);
	if (!image_data) {
		fprintf (stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf (
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	glGenTextures (1, tex);
    GLint uniform_text_location = glGetUniformLocation(shader_programme,"basic_texture");
    glUniform1i(uniform_text_location, 0);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, *tex);
	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap (GL_TEXTURE_2D);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	return true;
}

class Window : public core::utils::gl::GLFWWindow
{
private:
    core::gl::BaseObject *axis;
    core::gl::BaseObject *triangle;

    core::gl::BaseObject *deathStart;


    GLuint shader_programme;
    core::gl::Camera *cam;
    GLuint view_mat_location;
    GLuint matrix_location;


    obj_type death_star;
    //TRASFORMATION - moovment
    float matrix_no_move[18]
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    mat4 matrix;

    GLfloat speed = 1.0f;
    GLfloat last_position_x = 0.0f;
    GLfloat degree = 10.0f;

    mat4 rotation_matrix ;





public :
    Window()
    {



    }
    void setup()
    {
        // load texture



        matrix = mat4(
                     1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);

        rotation_matrix =   identity_mat4();
        rotation_matrix = rotate_x_deg(rotation_matrix, 0);
        cam = new core::gl::Camera(
            0.0f, 0.0f, 2.0f, //cam position
            0.1f, 100.0f, 67.0f, // near far fovy
            this->width, this->height
        );


        //AXIS RAW
        GLfloat points_axis[] =  {  -5.0f, 0.0f, 0.0f, //line x
                                    5.0f, 0.0f, 0.0f,

                                    0.0f, -5.0f, 0.0f, //line y
                                    0.0f,  5.0f, 0.0f,

                                    0.0f, 0.0f, -5.0f, //line z
                                    0.0f, 0.0f,  5.0f
                                 };

        GLfloat normal_axis[] = {         0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f
                                 };

        //Geometry settings TRIANGLE
        GLfloat points_triangle[] =  {
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};

        GLfloat normals_triangle[] =  {   0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f,
                                          0.0f, 0.0f, 1.0f
                                      };

        //AXIS
        GLuint axiz_size = 18;
        std::cout << 18*sizeof(GLfloat) << " \n";
        axis = new core::gl::BaseObject(points_axis, axiz_size, {}, 0, normal_axis, axiz_size, {}, 0);

        // 2^16 = 65536
	GLfloat texcoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};
        //TRIANGLE
        GLuint triangle_size = 18;
        std::cout << triangle_size*sizeof(GLfloat) << " \n";
        triangle = new core::gl::BaseObject(points_triangle, triangle_size, {}, 0, normals_triangle, triangle_size, texcoords, 12);


        //DEATh STAR LOADING
       /* Load3DS(&death_star, "death-star/deathstar.3ds");
        GLuint death_star_size = death_star.nobj[0].vertices_qty *3;
        deathStart = new core::gl::BaseObject(&death_star.nobj[0].vertex[0], death_star_size, {}, 0, {}, 0);

        //START SHADER PROGRAM SETUP ---------------------------------------------------------------------------
        /* GL shader objects for vertex and fragment shader [components] */
        GLuint vert_shader, frag_shader;
        /* GL shader programme object [combined, to link] */
        shader_programme = create_programme_from_files_gl (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);





        glLinkProgram( shader_programme );
        is_shader_programme_valid(shader_programme);
        glUseProgram( shader_programme );

        matrix_location = glGetUniformLocation(shader_programme, "model");
        view_mat_location = glGetUniformLocation (shader_programme, "view");
        GLuint proj_mat_location = glGetUniformLocation (shader_programme, "proj");

        GLuint tex;
        load_texture ("skulluvmap.png", &tex, shader_programme);


        glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix_no_move);
        glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, cam->getView().m);
        glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, cam->getProj().m);
        //--END SHADER PROGRAM SETUP -------------------------------------------------------------------------
    }
    void input()
    {
        // control keys and camera movment

        if (glfwGetKey (this->window, GLFW_KEY_A))
        {
            cam->traslation(core::gl::Camera::Axis::x, core::gl::Camera::Direction::minus, elapsed_seconds);
        }
        if (glfwGetKey (this->window, GLFW_KEY_D))
        {
            cam->traslation(core::gl::Camera::Axis::x, core::gl::Camera::Direction::plus, elapsed_seconds);
        }
        if (glfwGetKey (this->window, GLFW_KEY_Q))
        {
            cam->traslation(core::gl::Camera::Axis::y, core::gl::Camera::Direction::minus, elapsed_seconds);
        }
        if (glfwGetKey (this->window, GLFW_KEY_E))
        {
            cam->traslation(core::gl::Camera::Axis::y, core::gl::Camera::Direction::plus, elapsed_seconds);
        }
        if (glfwGetKey (this->window, GLFW_KEY_W))
        {
            cam->traslation(core::gl::Camera::Axis::z, core::gl::Camera::Direction::minus, elapsed_seconds);
        }
        if (glfwGetKey (this->window, GLFW_KEY_S))
        {
            cam->traslation(core::gl::Camera::Axis::z, core::gl::Camera::Direction::plus, elapsed_seconds);
        }

        if (glfwGetKey (this->window, GLFW_KEY_LEFT))
        {
            cam->yaw(core::gl::Camera::Direction::minus, elapsed_seconds);
        }
        if (glfwGetKey (this->window, GLFW_KEY_RIGHT))
        {
            cam->yaw(core::gl::Camera::Direction::plus, elapsed_seconds);
        }
        if (glfwGetKey (this->window, GLFW_KEY_UP))
        {
            cam->pitch(core::gl::Camera::Direction::minus, elapsed_seconds);
        }
        if (glfwGetKey (this->window, GLFW_KEY_DOWN))
        {
            cam->pitch(core::gl::Camera::Direction::plus, elapsed_seconds);
        }

        if (glfwGetKey (this->window, GLFW_KEY_Z))
        {
            cam->roll(core::gl::Camera::Direction::minus, elapsed_seconds);
        }

        if (glfwGetKey (this->window, GLFW_KEY_C))
        {
            cam->roll(core::gl::Camera::Direction::plus, elapsed_seconds);
        }

        if(cam->isMoved())
            glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, cam->getView().m);
    }

    void loop()
    {
        //matrix tralsation
        if(fabs(last_position_x) > 1.0f)
        {
            speed = -speed;
        }
        matrix.m[12] = elapsed_seconds * speed + last_position_x;
        matrix.m[14] = 0;
        last_position_x = matrix.m[12];

        if(degree > 360)
            degree = 0;
        degree+=0.0000005f;
        rotation_matrix = rotate_y_deg(rotation_matrix, degree);
        mat4 matrix_final = matrix* rotation_matrix;

        glUseProgram( shader_programme );


        glUniform1i(glGetUniformLocation(shader_programme, "texture"), false);
        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix_no_move);
        glBindVertexArray( axis->getVao() );
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays( GL_LINES, 0, 6 );


        glUniform1i(glGetUniformLocation(shader_programme, "texture"), true);
        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix_no_move);
        glBindVertexArray( triangle->getVao() );
        /* draw points 0-3 from the currently bound VAO with current in-use shader */
        glDrawArrays( GL_TRIANGLES, 0, 6);









/*
        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix_no_move);
        glBindVertexArray( deathStart->getVao() );
        glDrawArrays( GL_LINES, 0, death_star.nobj[0].vertices_qty );
*/
    }

    ~Window()
    {
        delete axis;
        delete triangle;
        delete deathStart;
        delete cam;
    }

};

int main ()
{
    // Create a sample listener and controller
    LeapListener listener;
    Controller controller;
    // Have the sample listener receive events from the controller
    controller.addListener(listener);

    Window window;
    window.init();
    window.execute();


    // Remove the sample listener when done
    controller.removeListener(listener);
    return 0;
}

