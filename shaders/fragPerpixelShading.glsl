#version 330 core
out vec4 FragColor;

in vec3 vFragPos;  
in vec3 vObjColor;  
in vec3 vNormal;

uniform vec3 uViewPos;
uniform float uShininess;
uniform float uAmbientStrength;

void main()
{
    vec3 lightColor=vec3(1.0f, 1.0f, 1.0f);
    vec3 direction=vec3(0.0f, 0.0f, -1.0f);

    // ambient
    vec3 ambient = uAmbientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(vNormal);
    // 平行光不需要计算物体和光源的位置
    vec3 lightDir = normalize(-direction);  

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;
    
    // specular
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
    vec3 specular =  spec * lightColor;
        
    vec3 result = (ambient + diffuse + specular)*vObjColor;
    FragColor = vec4(result, 1.0);
} 