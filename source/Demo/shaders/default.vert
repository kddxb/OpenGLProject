#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vColor;

out vec3 fColor;

void main()
{
	fColor = vColor;
	gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);
}