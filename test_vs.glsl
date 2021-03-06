#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec3 vertex_colour;
layout (location = 3) in vec2 vertex_texture;

uniform mat4  model, view, proj;


out vec3 position_eye, normal_eye, colour;
out vec2 texture_coordinates;

void main () {
   colour = vertex_colour;
   texture_coordinates = vertex_texture;
   position_eye = vec3(view * model * vec4(vertex_position, 1.0));
   normal_eye = vec3(view * model * vec4(vertex_normal, 0.0));
   gl_Position =  proj  * vec4(position_eye, 1.0);
}
