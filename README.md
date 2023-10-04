# Water-Ripple-Shader
## Summary

This project demonstrates a water ripple effect using OpenGL and GLSL. The effect is triggered by user interaction, specifically mouse clicks, which generate ripples on the water surface. Each ripple gradually fades away over time, creating a realistic water interaction effect. The ripple's intensity and fading are controlled by a fragment shader, part of a broader graphics pipeline managed in C++.



https://github.com/Nowwin/Water-Ripple-Shader/assets/36608434/03585188-a161-44dc-9d44-58e57da10473



## Features
- Water Ripple Effect: Realistic water ripple effect triggered by mouse clicks.
- Smooth Transitions: Utilizes the smoothstep function in the fragment shader to create smooth transitions in the ripple intensity, enhancing the visual appeal of the effect.
- User Interaction: The ripple effect is interactive and reacts to user mouse clicks.
- Shader-Based Rendering: The rendering is handled by a GLSL shader within a graphics pipeline managed in C++.

## Compilation

To build the project, simply run the `build.py` script:

```bash
python build.py
This will compile the project into an executable.
```


## Details
The core of the ripple effect lies in the GLSL fragment shader, defined below:

```
#version 410 core

in vec2 fragTexCoord;  // Fragment texture coordinates

// Uniforms updated from C++ code
uniform float time;  // Updated per frame
uniform vec2 interactionPoint;  // Updated based on user interaction
uniform float rippleStartTime;  // Time the ripple was created
uniform float decayRate;  // Decay rate

out vec4 fragColor;  // Fragment color output

void main()
{
    float distance = length(fragTexCoord - interactionPoint);  // Distance from interaction point
    float ripple = sin(20.0 * (distance - time));  // Ripple sine wave
    float decayFactor = exp(-decayRate * (time - rippleStartTime));  // Exponential decay factor
    float rippleIntensity = 0.1 * ripple / distance * decayFactor;  // Final ripple intensity
    float softRipple = smoothstep(0.0, 1.0, rippleIntensity);  // Soften ripple edges
    vec3 color = vec3(0.0, 0.5 + softRipple, 1.0);  // Ripple color (blue)

    fragColor = vec4(color, 1.0);  // Set fragment color
}

```
## Graphics Pipeline
The main C++ code manages the graphics pipeline, handling user input to trigger the ripple effect and updating the shader uniforms accordingly. When a mouse click event is detected, the interactionPoint and rippleStartTime uniforms are updated to create a new ripple at the click location. The time and decayRate uniforms are updated continuously to animate the ripple and control its fading over time.

## Texture
The project can be enhanced by adding a water texture to the rendering. A water texture can be loaded using a texture loading library such as stb_image, and blended with the ripple effect in the fragment shader to create a more realistic water appearance (Work in progress)
