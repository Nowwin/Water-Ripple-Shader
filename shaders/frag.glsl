#version 410 core
in vec2 fragTexCoord;
uniform float time; // Updated per frame
uniform vec2 interactionPoint; // Updated based on user interaction
uniform float rippleStartTime;  // Time the ripple was created
uniform float decayRate;  // Decay rate

out vec4 fragColor;

void main()
{
    float distance = length(fragTexCoord - interactionPoint);
    float ripple = sin(20.0 * (distance - time));
    float decayFactor = exp(-decayRate * (time - rippleStartTime));
    float rippleIntensity = 0.1 * ripple / distance * decayFactor;
    //float rippleIntensity = 0.1 * ripple / distance;
    float softRipple = smoothstep(0.0, 1.0, rippleIntensity);
    vec3 color = vec3(0.0, 0.5 + softRipple, 1.0);

    fragColor = vec4(color, 1.0);
}