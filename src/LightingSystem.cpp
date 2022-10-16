#include "LightingSystem.hpp"

// setters
void LightingSystem::setNewDirLight(const std::string &name, DirLight *newDirLight)
{
    dirLight.reset(newDirLight);
}
void LightingSystem::setNewPointLight(const std::string &name, PointLight *newPointLight)
{
    if (PointLightList.size() == 0) {
        pointLightModel = new Model("../../Model/lightbulb/scene.gltf");
    }
    PointLightList.insert(std::pair(name, std::make_unique<PointLight>()));
    PointLightList.at(name).reset(newPointLight);
}
void LightingSystem::setNewSpotLight(const std::string &name, SpotLight *newSpotLight)
{
}
// getters
DirLight *LightingSystem::getDirLight(std::string name) {}
PointLight *LightingSystem::getPointLight(std::string name) {return PointLightList.at(name).get();}
SpotLight *LightingSystem::getSpotLight(std::string name) {}

void LightingSystem::setupLighting(const Shader &shader)
{
    // Directional light
    if (dirLight != nullptr)
    {
        shader.setBool("hasDirLight", true);
        shader.setVec3("dirLight.direction", this->dirLight->getDirection());
        shader.setVec3("dirLight.ambient", this->dirLight->getAmbient());
        shader.setVec3("dirLight.diffuse", this->dirLight->getDiffuse());
        shader.setVec3("dirLight.specular", this->dirLight->getSpecular());
    }
    else
    {
        shader.setBool("hasDirLight", false);
    }

    // Point light
    if (PointLightList.size() > 0)
    {
        shader.setInt("NumberOfPointLight", PointLightList.size());
        int counter = 0;
        for (auto &pointLight : PointLightList)
        {
            shader.setVec3("pointLights[" + std::to_string(counter) + "].position", pointLight.second->getPosition());
            shader.setVec3("pointLights[" + std::to_string(counter) + "].ambient", pointLight.second->getAmbient());
            shader.setVec3("pointLights[" + std::to_string(counter) + "].diffuse", pointLight.second->getDiffuse());
            shader.setVec3("pointLights[" + std::to_string(counter) + "].specular", pointLight.second->getSpecular());
            shader.setFloat("pointLights[" + std::to_string(counter) + "].constant", pointLight.second->getConstant());
            shader.setFloat("pointLights[" + std::to_string(counter) + "].linear", pointLight.second->getLinear());
            shader.setFloat("pointLights[" + std::to_string(counter) + "].quadratic", pointLight.second->getQuadratic());
            counter++;
        }
    }
    else
    {
        shader.setInt("NumberOfPointLight", 0);
    }
}

void LightingSystem::draw(Shader *pLightShader)
{
    if (PointLightList.size() > 0)
    {

        for (auto &pointLight : PointLightList)
        {
            pointLightModel->model = glm::mat4(1.0f);
            pointLightModel->model = glm::translate(pointLightModel->model, pointLight.second->getPosition());
            pointLightModel->model = glm::rotate(pointLightModel->model, glm::radians(-90.0f), glm::vec3(1.f, 0.f, 0.f));
            pointLightModel->model = glm::scale(pointLightModel->model, glm::vec3(1.0f));
            pointLightModel->draw(pLightShader);
        }
    }
}
