#version 330 core

<<<<<<< HEAD
layout (location=0) in vec3 in_position;
layout (location=1) in vec3 in_color;

out vec3 our_color;


=======
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

out vec3 our_color;

>>>>>>> 958ba502ec3a2d1e5909de5e63d349b4e1d83627
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

<<<<<<< HEAD
void main(){
	gl_Position=projection * view * model * vec4(in_position,1.0);
	our_color=in_color;
=======
void main ()
{
    gl_Position = projection * view * model * vec4(in_position, 1.0);
	our_color = in_color;
>>>>>>> 958ba502ec3a2d1e5909de5e63d349b4e1d83627
}