#version 330 core

in vec4 ourColor;
in vec2 texCoord;
in vec3 normalDirection;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    color = texture(ourTexture, texCoord);
    
    //color = ourColor;
}
