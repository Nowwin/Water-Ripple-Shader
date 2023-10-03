#version 410 core
in vec2 fragTexCoord;
uniform float time; // Updated per frame in your C++ code
uniform vec2 interactionPoint; // Updated based on user interaction in your C++ code

out vec4 fragColor;

void main()
{
    float distance = length(fragTexCoord - interactionPoint);
    float ripple = sin(20.0 * (distance - time));
    float rippleIntensity = 0.1 * ripple / distance;
    
    vec3 color = vec3(0.0, 0.5 + rippleIntensity, 1.0); // Simple blue color with ripple intensity
    fragColor = vec4(color, 1.0);
}