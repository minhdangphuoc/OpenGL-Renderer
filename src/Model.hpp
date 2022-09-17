#pragma once
#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <cstdint> 
#include <vector>
#include <string>
#include <map>
#include <cstdint> 
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Material.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(std::string path)
    {
        loadModel(path);
    }
    void Draw(Shader *shader);	

    // model data 
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;
    std::string directory;
    bool gammaCorrection;

    void cleanMesh();

private:


    void loadModel(std::string const &path);
    Material loadMaterial(aiMaterial* mat);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             std::string typeName);
    uint32_t TextureFromFile(const char *path, const std::string &directory, bool gamma);

};

#endif