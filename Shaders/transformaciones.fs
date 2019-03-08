#version 330 core

<<<<<<< HEAD
in vec3 our_color;
out vec4 color;

void main(){
	color=vec4(our_color,1.0);
=======
 in vec3 our_color;
 out vec4 color;

void main ()
{
    color = vec4(our_color, 1.0);
>>>>>>> 958ba502ec3a2d1e5909de5e63d349b4e1d83627
}