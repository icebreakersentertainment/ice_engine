#version 330 core
  
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 pvmMatrix;
uniform mat3 normalMatrix;

layout (std140, binding = 0) uniform Bones
{
	mat4 bones[100];
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 textureCoordinate;
layout (location = 4) in ivec4 boneIds;
layout (location = 5) in vec4 boneWeights;

out vec4 ourColor;
out vec2 texCoord;
out vec3 normalDirection;

void main()
{
	// Calculate the transformation on the vertex position based on the bone weightings
	mat4 boneTransform = bones[ boneIds[0] ] * boneWeights[0];
    boneTransform     += bones[ boneIds[1] ] * boneWeights[1];
    boneTransform     += bones[ boneIds[2] ] * boneWeights[2];
    boneTransform     += bones[ boneIds[3] ] * boneWeights[3];
    
    //mat4 tempM = mat4(1.0);
    //boneTransform = tempM;
    
    // This is for animating the model
    vec4 tempPosition = boneTransform * vec4(position, 1.0);
	gl_Position = pvmMatrix * tempPosition;
	
	/*
	float sum = boneWeights[0] + boneWeights[1] + boneWeights[2] + boneWeights[3];
	if (sum > 1.01f)
		gl_Position = pvmMatrix * vec4(position, 1.0);
	else if (sum < 0.99f)
		gl_Position = pvmMatrix * vec4(position, 1.0);
	*/
	/*
	if (boneIds[0] > 99 || boneIds[0] < 0)
		gl_Position = pvmMatrix * vec4(position, 1.0);
	else if (boneIds[1] > 99 || boneIds[1] < 0)
		gl_Position = pvmMatrix * vec4(position, 1.0);
	else if (boneIds[2] > 99 || boneIds[2] < 0)
		gl_Position = pvmMatrix * vec4(position, 1.0);
	else if (boneIds[3] > 99 || boneIds[3] < 0)
		gl_Position = pvmMatrix * vec4(position, 1.0);
	*/
	// Calculate normal
	vec4 normalDirTemp = boneTransform * vec4(normal, 0.0);
	normalDirection = normalize(normalMatrix * normalDirTemp.xyz);
    
    //gl_Position = vec4(position, 1.0);
    //gl_Position = pvmMatrix * vec4(position, 1.0);
    ourColor = color;
	texCoord = textureCoordinate;
}
