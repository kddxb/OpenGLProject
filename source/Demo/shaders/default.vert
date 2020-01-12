#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 fColor;

void main()
{
	fColor = vec3(1, 1, 0);
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}