#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 vLightingColor;

uniform vec3 uViewPos;
uniform float uShininess;
uniform float uAmbientStrength;
uniform vec3 uDirection;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec3 FragPos = vec3(uModel * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(uModel))) * aNormal;  
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    vec3 ObjColor = aColor;

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    // ambient
    vec3 ambient = uAmbientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-uDirection);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;
    
    // specular
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
    vec3 specular =  spec * lightColor;
        
    vLightingColor = (ambient + diffuse + specular)*ObjColor;
}