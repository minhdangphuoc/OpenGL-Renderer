#version 410 core
#define NR_POINT_LIGHTS 2
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
     // Properties
    vec3 result = vec3(0.0);//needs an initial value or else the model won't render correctly
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    //Dir Light
    result = CalcDirLight(dirLight, material.ambient, material.diffuse, material.specular, material.shininess, norm, viewDir);

    //Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], material.ambient, material.diffuse, material.specular, material.shininess, norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
} 

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // Combine results
    ambient = light.ambient * ambient;
    diffuse = light.diffuse * diff * diffuse;
    specular = light.specular * spec * specular;

    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // Combine results
    ambient = light.ambient * ambient;
    diffuse = light.diffuse * diff * diffuse;
    specular = light.specular * spec * specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}