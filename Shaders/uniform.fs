#version 330 core
in vec4 ourColor;
out vec4 color;
<<<<<<< HEAD
//variable global de nombre globalColor
=======
//Variable global de color
>>>>>>> 517146ea66e6ffac2003ee833d831e454fafd539
uniform vec4 globalColor;
void main()
{
	color = globalColor * ourColor;
}