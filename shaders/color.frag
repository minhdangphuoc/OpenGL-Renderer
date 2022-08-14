#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform int isLightMap;
uniform float time;

vec3 calculate_emission()
{
    vec3 show = step(vec3(1.0), vec3(1.0) - texture(material.specularMap, TexCoords).rgb);
    return texture(material.emissionMap, TexCoords + vec2(0.0, time)).rgb * show;
}

void main()
{   
    if (isLightMap == 0)
    {
        // ambient
        vec3 ambient = light.ambient * material.ambient;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * material.diffuse);
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * material.specular);  
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    } 
    if (isLightMap == 1)
    {
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuseMap, TexCoords).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuseMap, TexCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specularMap, TexCoords).rgb;  
        
        // emission
        float speed = 0.5;
        vec3 emission = calculate_emission() * (abs(sin(time))); //Fade effect

        vec3 result = ambient + diffuse + specular + emission;
        FragColor = vec4(result, 1.0);
    }
} 