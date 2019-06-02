#version 330 core
/*struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};*/

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoords;
 
uniform vec3 viewPos;
uniform Light light;  
//uniform Material material;
uniform sampler2D materialDiff;
uniform sampler2D materialSpec;
uniform float materialShine;

void main()
{
    // Ambient
    vec3 ambient  = light.ambient * vec3(texture(materialDiff, TexCoords));
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * vec3(texture(materialDiff, TexCoords)));
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShine);
    vec3 specular = light.specular * (spec * vec3(texture(materialSpec, TexCoords)));  
        
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}