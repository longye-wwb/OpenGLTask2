#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform vec3 uLightDir; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;
uniform float uGlossy;

void main()
{             
    // Retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    
    vec3 LightDir = normalize(uLightDir);
    float Diff = max(dot(Normal, LightDir), 0.0);
    vec3 Diffuse = Diff * uLightColor;

    vec3 ViewDir = normalize(uViewPos - FragPos);
    vec3 HalfDir = normalize(LightDir + ViewDir);  
    float Spec = pow(max(dot(Normal, HalfDir), 0.0), uGlossy);
    vec3 Specular =  Spec * uLightColor;  

     vec3 Result =  Diffuse + Specular;
     FragColor = vec4(Result, 1.0);
}