#version 430 core
in vec3			fPosition;
in vec3			fNormal;
in vec3			fColor;

void main()
{
	gl_FragColor = vec4(fColor,1.0);
}