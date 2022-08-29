#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 objectCol;

void main()
{
	float Kambient = 0.1f;
	vec3 ambient = Kambient * lightCol;

	vec3 norm = normalize(Normal);
	vec3 lightDir = lightPos - FragPos;

	lightDir = normalize(lightDir);
	
	float Kdiffuse = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = Kdiffuse * lightCol;
	
	vec3 result = (ambient + diffuse) * objectCol;

	color = vec4(result, 1.0f);
}
