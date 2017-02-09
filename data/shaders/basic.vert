#version 330 core
#extension GL_ARB_shading_language_420pack : enable

uniform mat4 pvmMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;

void main()
{
	gl_Position = pvmMatrix * vec4(position, 1.0);
}
