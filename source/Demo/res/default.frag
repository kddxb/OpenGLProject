#version 330 core
in vec3			fPosition;
in vec3			fNormal;
in vec3			fColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	vec3 norm = normalize(fNormal);

	//������
	vec3 ambient = 0.2 * fColor;

	//�������
	vec3 lightDir = normalize(-lightDirection);
	float diff = pow(abs(dot(norm, lightDir)), 3.0);
	vec3 diffuse = lightColor * (diff * fColor);

	//����߹�
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fPosition);
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specular = specularStrength * spec * lightColor;

	//������+������� + ����߹�
	gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
	gl_FragColor = vec4(fColor,1.0);
}