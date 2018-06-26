// Adapted from: https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/5.advanced_lighting/8.1.deferred_shading/8.1.g_buffer.vs
#version 440 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 pvmMatrix;
uniform mat3 normalMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 textureCoordinate;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

void main()
{
    vec4 worldPos = modelMatrix * vec4(position, 1.0);
    FragPos = worldPos.xyz; 
    TexCoords = textureCoordinate;
    
    mat3 normalMatrix2 = transpose(inverse(mat3(modelMatrix)));
    Normal = normalMatrix2 * normal;
    
    gl_Position = pvmMatrix * vec4(position, 1.0);

    //gl_Position = projection * view * worldPos;
    
	//texCoord = textureCoordinate;
}
