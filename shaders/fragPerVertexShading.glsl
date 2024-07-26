#version 460 core
out vec4 FragColor;

in vec3 vLightingColor; 

void main()
{
    FragColor = vec4(vLightingColor, 1.0);
 } 