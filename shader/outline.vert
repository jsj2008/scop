#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt;

uniform mat4 MVP;
uniform mat4 M;

out vec3 position;
out vec3 normal;
out vec2 uv;

void main()
{
	normal = mat3(transpose(inverse(M))) * vn;
	position = vec3(M * vec4(vertexPos, 1.f));
	gl_Position =  MVP * vec4(vec3((vertexPos + (normalize(vn) * 0.2))), 1.0f);
	uv = vt;
}