#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec4 worldPos = uModel * vec4(position, 1.0f);
    FragPos = worldPos.xyz; 
    gl_Position = uProjection * uView * worldPos;

    
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    Normal = normalMatrix * normal;
}