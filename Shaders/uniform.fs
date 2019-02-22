#version 330 core
in vec4 ourColor;
out vec4 color;
//Variable global de color
uniform vec4 globalColor;
void main()
{
	color = globalColor * ourColor;
}