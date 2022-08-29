#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

void main(void)
{
	vec4 worldPos = model * vec4(vertex, 1.0f);
	gl_Position = projection * view * worldPos;
	FragPos = worldPos.xyz;
	Normal = mat3(normalMatrix) * normal;
}
