#include <vector>

#include "models/Model.cpp"
#include "lights/Light.cpp"
#include "lights/DirLight.cpp"
#include "lights/PointLight.cpp"
#include "models/Box.cpp"
#include "Shader.cpp"
#include "models/LoadedModel.cpp"
#include "Renderer.cpp"
#include "Camera.cpp"

class Level {
public:
    std::vector<Light*>* lights;

    std::vector<Model*>* modelsForLightShader;
    std::vector<Model*>* modelsForModelShader;
    std::vector<Model*>* modelsForOurShader;
    std::vector<std::pair<Shader*, std::vector<Model*>*>*>* toRender;

    GLFWwindow *window;

    Shader* ourShader;
    Shader* lightShader;
    Shader* modelShader;

    Renderer* renderer;


    Level(int SCR_WIDTH, int SCR_HEIGHT, GLFWwindow *window) {
        this->window = window;
        ourShader = new Shader("src/model/shaders/shader.vs", "src/model/shaders/shader.fs");
        lightShader = new Shader("src/model/shaders/lightShader.vs", "src/model/shaders/lightShader.fs");
        modelShader = new Shader("src/model/shaders/modelShader.vs", "src/model/shaders/modelShader.fs");

        lights = new std::vector<Light*>();
        modelsForLightShader = new std::vector<Model*>();
        modelsForOurShader = new std::vector<Model*>();
        modelsForModelShader = new std::vector<Model*>();

        //createDirLight();
        createPointLights();
        createModels();
        createBoxes();

        std::pair<Shader*, std::vector<Model*>*>* lightShaderModels = new std::pair<Shader*, std::vector<Model*>*>(lightShader, modelsForLightShader);
        std::pair<Shader*, std::vector<Model*>*>* modelShaderModels = new std::pair<Shader*, std::vector<Model*>*>(ourShader, modelsForModelShader);
        std::pair<Shader*, std::vector<Model*>*>* ourShaderModels = new std::pair<Shader*, std::vector<Model*>*>(ourShader, modelsForOurShader);

        renderer = new Renderer(SCR_WIDTH, SCR_HEIGHT);
        toRender = new std::vector<std::pair<Shader*, std::vector<Model*>*>*>();
        toRender->push_back(lightShaderModels);
        toRender->push_back(ourShaderModels);
        toRender->push_back(modelShaderModels);
    }

    void render(Camera* camera) {
        processAction(window);
        renderer->render(toRender, lights, camera);
    }

protected:

    void createDirLight() {
        DirLight* dirLight = new DirLight();
        dirLight->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
        dirLight->setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
        dirLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        dirLight->setDirection(glm::vec3(0.0f, -1000.0f, 0.0f));
        lights->push_back(dirLight);
    }

    void createPointLights() {
        std::vector<PointLight*>* pointLights = new std::vector<PointLight*>();
        pointLights->push_back(createPointLight(glm::vec3(0.0f, 0.0f, 0.0f)));
        pointLights->push_back(createPointLight(glm::vec3(1.0f, 1.0f, 1.0f)));

        for(PointLight* pointLight : *pointLights) {
            modelsForLightShader->push_back(pointLight->getModel());
            lights->push_back(pointLight);
        }
    }

    PointLight* createPointLight(glm::vec3 position) {
        PointLight* pointLight = new PointLight();
        pointLight->setPosition(position);
        pointLight->setModel(createBox(pointLight->getPosition()));
        pointLight->scaleModel(glm::vec3(0.5f, 0.5f, 0.5f));
        pointLight->setAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
        pointLight->setDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
        pointLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLight->setConstant(1.0f);
        pointLight->setLinear(0.09f);
        pointLight->setQuadratic(0.032f);   
        return pointLight;    
    }

    Box* createBox(glm::vec3 position) {
        Box* box = new Box();
        box->setPosition(position);
        box->setMaterial(new Material(
            new Texture("src/model/assets/container2.png", true),
            new Texture("src/model/assets/container2_specular.png", true),
            32.0f
        ));
        return box;
    }

    void createModels() {
        //Model* backpack = new LoadedModel("src/model/assets/backpack/backpack.obj", new Texture("src/model/assets/backpack/diffuse.jpg", false));
        Model* bird = new LoadedModel("src/model/assets/bird/bird.obj", new Texture("src/model/assets/bird/diffuse.jpg", false));
        Model* knight = new LoadedModel("src/model/assets/knight/knight2.obj", new Texture("src/model/assets/knight/armor.jpg", false));
        Model* penguin = new LoadedModel("src/model/assets/penguin/penguin.obj", new Texture("src/model/assets/penguin/PenguinDiffuseColor.png", true));

        //backpack->setPosition(glm::vec3(6, 0, 0));
        bird->setPosition(glm::vec3(15, 0, 0));
        knight->setPosition(glm::vec3(-8, 0 ,0));
        penguin->setPosition(glm::vec3(-1, 0, 0));

        //modelsForModelShader->push_back(backpack);
        modelsForModelShader->push_back(bird);
        modelsForModelShader->push_back(knight);
        modelsForModelShader->push_back(penguin);
    }

    void createBoxes() {
        modelsForOurShader->push_back(createBox(glm::vec3( 2.0f,  5.0f, -15.0f)));
        modelsForOurShader->push_back(createBox(glm::vec3(-1.5f, -2.2f, -2.5f)));
        modelsForOurShader->push_back(createBox(glm::vec3(-3.8f, -2.0f, -12.3f)));
        modelsForOurShader->push_back(createBox(glm::vec3( 2.4f, -0.4f, -3.5f)));
        modelsForOurShader->push_back(createBox(glm::vec3(-1.7f,  3.0f, -7.5f)));
        modelsForOurShader->push_back(createBox(glm::vec3( 1.3f, -2.0f, -2.5f)));
        modelsForOurShader->push_back(createBox(glm::vec3( 1.5f,  2.0f, -2.5f)));
        modelsForOurShader->push_back(createBox(glm::vec3( 1.5f,  0.2f, -1.5f)));
    }

    void processAction(GLFWwindow *window) {
        const float speed = 0.5f;
        PointLight* light = (PointLight*)lights->at(1);


        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            light->position.x -= speed;
            light->model->position.x -= speed;
        }
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            light->position.x += speed;
            light->model->position.x += speed;
        }
        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            light->position.y -= speed;
            light->model->position.y -= speed;
        }
        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            light->position.y += speed;
            light->model->position.y += speed;
        }
    }
};