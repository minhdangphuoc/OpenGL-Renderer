#version 410 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 inBoneIds; 
layout(location = 6) in vec4 inWeights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(inBoneIds[i] == -1) 
            continue;
        if(inBoneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(inPos,1.0f);
            totalNormal = mat3(transpose(inverse(model))) * inNormal;
            break;
        }
        vec4 localPosition = finalBonesMatrices[inBoneIds[i]] * vec4(inPos,1.0f);
        totalPosition += localPosition * inWeights[i];
        vec3 localNormal = mat3(finalBonesMatrices[inBoneIds[i]]) * inNormal;
        totalNormal += localNormal;
    }
    
    FragPos = vec3(model * totalPosition);
    Normal = totalNormal;  
    TexCoords = inTexCoords;
    
    mat4 viewModel = view * model;

    gl_Position = projection * mat4(view * model) * totalPosition;
}