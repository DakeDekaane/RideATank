#version 330 core

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoords;
 
uniform vec3 viewPos;

uniform DirLight dirLight;  
#define POINT_LIGHTS 24
uniform PointLight pointLights[POINT_LIGHTS];
#define SPOTLIGHTS 2
uniform SpotLight spotLights[SPOTLIGHTS];

uniform sampler2D materialDiff;
uniform sampler2D materialSpec;
uniform float materialShine;

vec3 calcDirLight(DirLight light) {
	//Ambient
	vec3 ambient  = light.ambient * vec3(texture(materialDiff, TexCoords));
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * vec3(texture(materialDiff, TexCoords)));
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShine);
    vec3 specular = light.specular * (spec * vec3(texture(materialSpec, TexCoords)));  
        
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light) {
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

	float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light) {
	vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
        
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   

    // Ambient
    vec3 ambient  = light.ambient * vec3(texture(materialDiff, TexCoords));
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * vec3(texture(materialDiff, TexCoords)));
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShine);
    vec3 specular = light.specular * (spec * vec3(texture(materialSpec, TexCoords)));  

    diffuse *= intensity;
    specular *= intensity;

	float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
	vec3 result	 = vec3(0.0);

	//Directional lighting
	result += calcDirLight(dirLight);

	//Point lighting
	for (int i = 0; i < POINT_LIGHTS; i++) {
		result += calcPointLight(pointLights[i]);
	}

	//Spotlight lighting
	for (int i = 0; i < SPOTLIGHTS; i++) {
		result += calcSpotLight(spotLights[i]);
	}

	color = vec4(result, 1.0);
}