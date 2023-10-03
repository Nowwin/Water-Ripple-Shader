#version 410 core


layout(location = 0) in vec3 inPosition; // Vertex position
layout(location = 1) in vec2 inTexCoord; // Texture coordinate

out vec2 fragTexCoord;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}