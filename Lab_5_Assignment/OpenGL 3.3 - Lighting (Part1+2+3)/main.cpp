//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "sphere.h"
#include "Cone2.h"
#include "cube.h"
#include "hexagon.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "stb_image.h"
#include "cylinder.h"


#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

glm::mat4 RotationMatricesX(float theta);
glm::mat4 RotationMatricesY(float theta);
glm::mat4 RotationMatricesZ(float theta);

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 900;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = -90.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 0.0;
float translate_X = 10.0;
float translate_Y = -3.0;
float translate_Z = -5.0;
float scale_X = 3.0;
float scale_Y = .5;
float scale_Z = 1.0;


float r_fan = 0.0;
float r_sphere = 0.0;

bool fanOn = false;
bool directionalLightOn = true;
bool spotLightOn = true;



// camera

Camera camera(glm::vec3(-2.0f, 0.5f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 2.0, eyeZ = -10.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(4.0f,  2.89f,  2.0f),
    glm::vec3(2.0f,  2.89f,  3.8f),
    glm::vec3(1.3f, 2.95f, 1.36f),
    glm::vec3(1.5f, 1.1f, 1.0f)
};


//glm::vec3(-0.5, 1, -0.5)


PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool onOffToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

void drawChairWithTransformations(const glm::mat4& identityMatrix,
    const glm::mat4& globalTranslationMatrix,
    const glm::vec3& translation,
    const glm::vec3& rotation,
    Shader& lightingShaderWithTexture,
    Shader& ourShader,
    CylinderWithTexture& cylinder_window,
    Cube& cube_floor,
    const glm::vec3& viewPos) {

    // Start with the identity matrix for the whole chair
    glm::mat4 chairTransformMatrix = glm::translate(identityMatrix, translation);

    // Apply rotation (on all axes)
    chairTransformMatrix = glm::rotate(chairTransformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    chairTransformMatrix = glm::rotate(chairTransformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    chairTransformMatrix = glm::rotate(chairTransformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Keep the original structure for the chair as it was
    glm::mat4 translateMatrix, scaleMatrix, model;

    // Vertical cylinders (legs)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 0.8f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
    model = globalTranslationMatrix * chairTransformMatrix * scaleMatrix;
    cylinder_window.drawCylinder(lightingShaderWithTexture, model, viewPos);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 0.8f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    cylinder_window.drawCylinder(lightingShaderWithTexture, model, viewPos);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 0.8f, 6.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    cylinder_window.drawCylinder(lightingShaderWithTexture, model, viewPos);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 0.8f, 6.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    cylinder_window.drawCylinder(lightingShaderWithTexture, model, viewPos);

    // Seat (cube)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, 1.5f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 0.2f, 1.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest (cube)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, 1.6f, 6.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Armrests (cubes)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1f, 1.6f, 6.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest vertical sections
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, 2.0f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 0.1f, 1.0f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1f, 2.0f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 0.1f, 1.0f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Seat bottom part
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, 2.0f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 0.8f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);
}

void drawTableWithTransformations(const glm::mat4& identityMatrix,
    const glm::mat4& globalTranslationMatrix,
    const glm::vec3& translation,
    const glm::vec3& rotation,
    Shader& lightingShaderWithTexture,
    Shader& ourShader,
    Cube& cube_floor) {

    // Start with the identity matrix for the whole table
    glm::mat4 tableTransformMatrix = glm::translate(identityMatrix, translation);

    // Apply rotation (on all axes)
    tableTransformMatrix = glm::rotate(tableTransformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    tableTransformMatrix = glm::rotate(tableTransformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    tableTransformMatrix = glm::rotate(tableTransformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Keep the original structure for the table as it was
    glm::mat4 translateMatrix, scaleMatrix, model;

    // Table top (rectangular surface)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.7f, 2.0f, 5.7f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(2.0f, 0.1f, 1.5f));
    model = globalTranslationMatrix*tableTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Table leg 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.7f, 0.0f, 6.25f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.0f, 0.2f));
    model = globalTranslationMatrix*tableTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Table leg 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.4f, 0.0f, 5.9f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.0f, 0.1f, 1.0f));
    model = globalTranslationMatrix*tableTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);
}


void drawSofaWithTransformations(const glm::mat4& identityMatrix,
    const glm::mat4& globalTranslationMatrix,
    const glm::vec3& translation,
    const glm::vec3& rotation,
    Shader& lightingShaderWithTexture,
    Shader& ourShader,
    Cube& cube_floor,
    Cube& cube_box) {

    // Start with the identity matrix for the whole sofa
    glm::mat4 sofaTransformMatrix = glm::translate(identityMatrix, translation);

    // Apply rotation (on all axes)
    sofaTransformMatrix = glm::rotate(sofaTransformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    sofaTransformMatrix = glm::rotate(sofaTransformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    sofaTransformMatrix = glm::rotate(sofaTransformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 translateMatrix, scaleMatrix, model;

    // Sofa parts (like seat cushions, backrest, and armrests)
    // Seat 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 2.0f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 0.2f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Seat 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, -0.01f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 0.2f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, -.01f, 6.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.1f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, -.01f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.1f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Armrest 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, 2.0f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 0.2f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Armrest 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, -0.01f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 0.2f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest (large part)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, -.01f, 6.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.1f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest (small part)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, -.01f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.1f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Bottom Seat Cushion 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 1.0f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(4.0f, 0.2f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Bottom Seat Cushion 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 1.0f, 6.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(4.0f, 0.2f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Side Cushion 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.2f, 1.2f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.8f, 0.6f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_box.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Side Cushion 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.2f, 1.8f, 6.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.8f, 1.6f, 0.6f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_box.drawCubeWithTexture(lightingShaderWithTexture, model);
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        //Uper Plane
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };

    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    float cylinder_vertices[] = {
0.505450490764242,0.0f,0.494505494505494f,0.0f,0.0f,0.0f,
0.512152956078189,0.0f,0.797488226059654f,0.0f,0.0f,0.0f,
0.480997862840319,0.0f,0.795918367346938f,0.0f,0.0f,0.0f,
0.452958034399956,0.0f,0.794348508634222f,0.0f,0.0f,0.0f,
0.420230637186578,0.0f,0.783359497645211f,0.0f,0.0f,0.0f,
0.385948052837238,0.0f,0.773940345368916f,0.0f,0.0f,0.0f,
0.354773397497029,0.0f,0.759811616954474f,0.0f,0.0f,0.0f,
0.32670667116595,0.0f,0.740973312401883f,0.0f,0.0f,0.0f,
0.295514898986194,0.0f,0.71585557299843f,0.0f,0.0f,0.0f,
0.268998469265491,0.0f,0.692307692307692f,0.0f,0.0f,0.0f,
0.247147600952674,0.0f,0.664050235478806f,0.0f,0.0f,0.0f,
0.226844583987441,0.0f,0.629513343799058f,0.0f,0.0f,0.0f,
0.214332174278769,0.0f,0.596546310832025f,0.0f,0.0f,0.0f,
0.201814874044513,0.0f,0.56043956043956f,0.0f,0.0f,0.0f,
0.195523212879688,0.0f,0.521193092621663f,0.0f,0.0f,0.0f,
0.195471862361047,0.0f,0.48822605965463f,0.0f,0.0f,0.0f,
0.20164859617463,0.0f,0.453689167974882f,0.0f,0.0f,0.0f,
0.207822884725421,0.0f,0.417582417582417f,0.0f,0.0f,0.0f,
0.220225257608435,0.0f,0.379905808477236f,0.0f,0.0f,0.0f,
0.234204824992541,0.0f,0.354788069073783f,0.0f,0.0f,0.0f,
0.249742024775402,0.0f,0.329670329670329f,0.0f,0.0f,0.0f,
0.268394489355771,0.0f,0.304552590266876f,0.0f,0.0f,0.0f,
0.291724741657985,0.0f,0.282574568288853f,0.0f,0.0f,0.0f,
0.313502252087031,0.0f,0.263736263736263f,0.0f,0.0f,0.0f,
0.336837394914831,0.0f,0.244897959183673f,0.0f,0.0f,0.0f,
0.361742396455347,0.0f,0.233908948194662f,0.0f,0.0f,0.0f,
0.389760217530578,0.0f,0.221350078492935f,0.0f,0.0f,0.0f,
0.416225296732639,0.0f,0.21193092621664f,0.0f,0.0f,0.0f,
0.448918460266924,0.0f,0.200941915227629f,0.0f,0.0f,0.0f,
0.478508585317664,0.0f,0.197802197802197f,0.0f,0.0f,0.0f,
0.511218865691495,0.0f,0.197802197802197f,0.0f,0.0f,0.0f,
0.540816326530612,0.0f,0.199372056514913f,0.0f,0.0f,0.0f,
0.571976310294067,0.0f,0.204081632653061f,0.0f,0.0f,0.0f,
0.601583552184353,0.0f,0.21193092621664f,0.0f,0.0f,0.0f,
0.631198129863016,0.0f,0.224489795918367f,0.0f,0.0f,0.0f,
0.660815152804471,0.0f,0.23861852433281f,0.0f,0.0f,0.0f,
0.684208981939288,0.0f,0.2574568288854f,0.0f,0.0f,0.0f,
0.713838231194706,0.0f,0.279434850863422f,0.0f,0.0f,0.0f,
0.734124131320392,0.0f,0.30298273155416f,0.0f,0.0f,0.0f,
0.754414921971664,0.0f,0.329670329670329f,0.0f,0.0f,0.0f,
0.773150525486973,0.0f,0.357927786499215f,0.0f,0.0f,0.0f,
0.78877575473036,0.0f,0.389324960753532f,0.0f,0.0f,0.0f,
0.798175344904316,0.0f,0.42386185243328f,0.0f,0.0f,0.0f,
0.80757248981548,0.0f,0.456828885400313f,0.0f,0.0f,0.0f,
0.809193699046836,0.0f,0.497645211930926f,0.0f,0.0f,0.0f,
0.806124894242384,0.0f,0.527472527472527f,0.0f,0.0f,0.0f,
0.803056089437931,0.0f,0.557299843014128f,0.0f,0.0f,0.0f,
0.793759200301256,0.0f,0.588697017268445f,0.0f,0.0f,0.0f,
0.782909569291411,0.0f,0.623233908948194f,0.0f,0.0f,0.0f,
0.76426444049942,0.0f,0.653061224489795f,0.0f,0.0f,0.0f,
0.74405678878309,0.0f,0.679748822605965f,0.0f,0.0f,0.0f,
0.725401878939929,0.0f,0.703296703296703f,0.0f,0.0f,0.0f,
0.702074071900507,0.0f,0.72684458398744f,0.0f,0.0f,0.0f,
0.672508399477691,0.0f,0.745682888540031f,0.0f,0.0f,0.0f,
0.646055546589592,0.0f,0.762951334379905f,0.0f,0.0f,0.0f,
0.613364828318099,0.0f,0.775510204081632f,0.0f,0.0f,0.0f,
0.582229297182568,0.0f,0.786499215070643f,0.0f,0.0f,0.0f,
0.54641597832519,0.0f,0.794348508634222f,0.0f,0.0f,0.0f,

0.505450490764242,1.0f,0.494505494505494f,0.0f,0.0f,0.0f,
0.512152956078189,1.0f,0.797488226059654f,0.0f,0.0f,0.0f,
0.480997862840319,1.0f,0.795918367346938f,0.0f,0.0f,0.0f,
0.452958034399956,1.0f,0.794348508634222f,0.0f,0.0f,0.0f,
0.420230637186578,1.0f,0.783359497645211f,0.0f,0.0f,0.0f,
0.385948052837238,1.0f,0.773940345368916f,0.0f,0.0f,0.0f,
0.354773397497029,1.0f,0.759811616954474f,0.0f,0.0f,0.0f,
0.32670667116595,1.0f,0.740973312401883f,0.0f,0.0f,0.0f,
0.295514898986194,1.0f,0.71585557299843f,0.0f,0.0f,0.0f,
0.268998469265491,1.0f,0.692307692307692f,0.0f,0.0f,0.0f,
0.247147600952674,1.0f,0.664050235478806f,0.0f,0.0f,0.0f,
0.226844583987441,1.0f,0.629513343799058f,0.0f,0.0f,0.0f,
0.214332174278769,1.0f,0.596546310832025f,0.0f,0.0f,0.0f,
0.201814874044513,1.0f,0.56043956043956f,0.0f,0.0f,0.0f,
0.195523212879688,1.0f,0.521193092621663f,0.0f,0.0f,0.0f,
0.195471862361047,1.0f,0.48822605965463f,0.0f,0.0f,0.0f,
0.20164859617463,1.0f,0.453689167974882f,0.0f,0.0f,0.0f,
0.207822884725421,1.0f,0.417582417582417f,0.0f,0.0f,0.0f,
0.220225257608435,1.0f,0.379905808477236f,0.0f,0.0f,0.0f,
0.234204824992541,1.0f,0.354788069073783f,0.0f,0.0f,0.0f,
0.249742024775402,1.0f,0.329670329670329f,0.0f,0.0f,0.0f,
0.268394489355771,1.0f,0.304552590266876f,0.0f,0.0f,0.0f,
0.291724741657985,1.0f,0.282574568288853f,0.0f,0.0f,0.0f,
0.313502252087031,1.0f,0.263736263736263f,0.0f,0.0f,0.0f,
0.336837394914831,1.0f,0.244897959183673f,0.0f,0.0f,0.0f,
0.361742396455347,1.0f,0.233908948194662f,0.0f,0.0f,0.0f,
0.389760217530578,1.0f,0.221350078492935f,0.0f,0.0f,0.0f,
0.416225296732639,1.0f,0.21193092621664f,0.0f,0.0f,0.0f,
0.448918460266924,1.0f,0.200941915227629f,0.0f,0.0f,0.0f,
0.478508585317664,1.0f,0.197802197802197f,0.0f,0.0f,0.0f,
0.511218865691495,1.0f,0.197802197802197f,0.0f,0.0f,0.0f,
0.540816326530612,1.0f,0.199372056514913f,0.0f,0.0f,0.0f,
0.571976310294067,1.0f,0.204081632653061f,0.0f,0.0f,0.0f,
0.601583552184353,1.0f,0.21193092621664f,0.0f,0.0f,0.0f,
0.631198129863016,1.0f,0.224489795918367f,0.0f,0.0f,0.0f,
0.660815152804471,1.0f,0.23861852433281f,0.0f,0.0f,0.0f,
0.684208981939288,1.0f,0.2574568288854f,0.0f,0.0f,0.0f,
0.713838231194706,1.0f,0.279434850863422f,0.0f,0.0f,0.0f,
0.734124131320392,1.0f,0.30298273155416f,0.0f,0.0f,0.0f,
0.754414921971664,1.0f,0.329670329670329f,0.0f,0.0f,0.0f,
0.773150525486973,1.0f,0.357927786499215f,0.0f,0.0f,0.0f,
0.78877575473036,1.0f,0.389324960753532f,0.0f,0.0f,0.0f,
0.798175344904316,1.0f,0.42386185243328f,0.0f,0.0f,0.0f,
0.80757248981548,1.0f,0.456828885400313f,0.0f,0.0f,0.0f,
0.809193699046836,1.0f,0.497645211930926f,0.0f,0.0f,0.0f,
0.806124894242384,1.0f,0.527472527472527f,0.0f,0.0f,0.0f,
0.803056089437931,1.0f,0.557299843014128f,0.0f,0.0f,0.0f,
0.793759200301256,1.0f,0.588697017268445f,0.0f,0.0f,0.0f,
0.782909569291411,1.0f,0.623233908948194f,0.0f,0.0f,0.0f,
0.76426444049942,1.0f,0.653061224489795f,0.0f,0.0f,0.0f,
0.74405678878309,1.0f,0.679748822605965f,0.0f,0.0f,0.0f,
0.725401878939929,1.0f,0.703296703296703f,0.0f,0.0f,0.0f,
0.702074071900507,1.0f,0.72684458398744f,0.0f,0.0f,0.0f,
0.672508399477691,1.0f,0.745682888540031f,0.0f,0.0f,0.0f,
0.646055546589592,1.0f,0.762951334379905f,0.0f,0.0f,0.0f,
0.613364828318099,1.0f,0.775510204081632f,0.0f,0.0f,0.0f,
0.582229297182568,1.0f,0.786499215070643f,0.0f,0.0f,0.0f,
0.54641597832519,1.0f,0.794348508634222f,0.0f,0.0f,0.0f,
    };

    unsigned int cone_indices[] = {
0, 1, 2,

58, 59, 60,
58, 60, 61,
58, 61, 62,
58, 62, 63,
58, 63, 64,
58, 64, 65,
58, 65, 66,
58, 66, 67,
58, 67, 68,
58, 68, 69,
58, 69, 70,
58, 70, 71,
58, 71, 72,
58, 72, 73,
58, 73, 74,
58, 74, 75,
58, 75, 76,
58, 76, 77,
58, 77, 78,
58, 78, 79,
58, 79, 80,
58, 80, 81,
58, 81, 82,
58, 82, 83,
58, 83, 84,
58, 84, 85,
58, 85, 86,
58, 86, 87,
58, 87, 88,
58, 88, 89,
58, 89, 90,
58, 90, 91,
58, 91, 92,
58, 92, 93,
58, 93, 94,
58, 94, 95,
58, 95, 96,
58, 96, 97,
58, 97, 98,
58, 98, 99,
58, 99, 100,
58, 100, 101,
58, 101, 102,
58, 102, 103,
58, 103, 104,
58, 104, 105,
58, 105, 106,
58, 106, 107,
58, 107, 108,
58, 108, 109,
58, 109, 110,
58, 110, 111,
58, 111, 112,
58, 112, 113,
58, 113, 114,
58, 114, 115,
58, 115, 59,

0, 59, 60,
0, 60, 61,
0, 61, 62,
0, 62, 63,
0, 63, 64,
0, 64, 65,
0, 65, 66,
0, 66, 67,
0, 67, 68,
0, 68, 69,
0, 69, 70,
0, 70, 71,
0, 71, 72,
0, 72, 73,
0, 73, 74,
0, 74, 75,
0, 75, 76,
0, 76, 77,
0, 77, 78,
0, 78, 79,
0, 79, 80,
0, 80, 81,
0, 81, 82,
0, 82, 83,
0, 83, 84,
0, 84, 85,
0, 85, 86,
0, 86, 87,
0, 87, 88,
0, 88, 89,
0, 89, 90,
0, 90, 91,
0, 91, 92,
0, 92, 93,
0, 93, 94,
0, 94, 95,
0, 95, 96,
0, 96, 97,
0, 97, 98,
0, 98, 99,
0, 99, 100,
0, 100, 101,
0, 101, 102,
0, 102, 103,
0, 103, 104,
0, 104, 105,
0, 105, 106,
0, 106, 107,
0, 107, 108,
0, 108, 109,
0, 109, 110,
0, 110, 111,
0, 111, 112,
0, 112, 113,
0, 113, 114,
0, 114, 115,
    };

    unsigned int cylinder_indices[] = {
0, 1, 2,
0, 2, 3,
0, 3, 4,
0, 4, 5,
0, 5, 6,
0, 6, 7,
0, 7, 8,
0, 8, 9,
0, 9, 10,
0, 10, 11,
0, 11, 12,
0, 12, 13,
0, 13, 14,
0, 14, 15,
0, 15, 16,
0, 16, 17,
0, 17, 18,
0, 18, 19,
0, 19, 20,
0, 20, 21,
0, 21, 22,
0, 22, 23,
0, 23, 24,
0, 24, 25,
0, 25, 26,
0, 26, 27,
0, 27, 28,
0, 28, 29,
0, 29, 30,
0, 30, 31,
0, 31, 32,
0, 32, 33,
0, 33, 34,
0, 34, 35,
0, 35, 36,
0, 36, 37,
0, 37, 38,
0, 38, 39,
0, 39, 40,
0, 40, 41,
0, 41, 42,
0, 42, 43,
0, 43, 44,
0, 44, 45,
0, 45, 46,
0, 46, 47,
0, 47, 48,
0, 48, 49,
0, 49, 50,
0, 50, 51,
0, 51, 52,
0, 52, 53,
0, 53, 54,
0, 54, 55,
0, 55, 56,
0, 56, 57,
0, 57, 1,

58, 59, 60,
58, 60, 61,
58, 61, 62,
58, 62, 63,
58, 63, 64,
58, 64, 65,
58, 65, 66,
58, 66, 67,
58, 67, 68,
58, 68, 69,
58, 69, 70,
58, 70, 71,
58, 71, 72,
58, 72, 73,
58, 73, 74,
58, 74, 75,
58, 75, 76,
58, 76, 77,
58, 77, 78,
58, 78, 79,
58, 79, 80,
58, 80, 81,
58, 81, 82,
58, 82, 83,
58, 83, 84,
58, 84, 85,
58, 85, 86,
58, 86, 87,
58, 87, 88,
58, 88, 89,
58, 89, 90,
58, 90, 91,
58, 91, 92,
58, 92, 93,
58, 93, 94,
58, 94, 95,
58, 95, 96,
58, 96, 97,
58, 97, 98,
58, 98, 99,
58, 99, 100,
58, 100, 101,
58, 101, 102,
58, 102, 103,
58, 103, 104,
58, 104, 105,
58, 105, 106,
58, 106, 107,
58, 107, 108,
58, 108, 109,
58, 109, 110,
58, 110, 111,
58, 111, 112,
58, 112, 113,
58, 113, 114,
58, 114, 115,
58, 115, 59,

1, 2, 59,
59, 60, 2,
2, 3, 60,
60, 61, 3,
3, 4, 61,
61, 62, 4,
4, 5, 62,
62, 63, 5,
5, 6, 63,
63, 64, 6,
6, 7, 64,
64, 65, 7,
7, 8, 65,
65, 66, 8,
8, 9, 66,
66, 67, 9,
9, 10, 67,
67, 68, 10,
10, 11, 68,
68, 69, 11,
11, 12, 69,
69, 70, 12,
12, 13, 70,
70, 71, 13,
13, 14, 71,
71, 72, 14,
14, 15, 72,
72, 73, 15,
15, 16, 73,
73, 74, 16,
16, 17, 74,
74, 75, 17,
17, 18, 75,
75, 76, 18,
18, 19, 76,
76, 77, 19,
19, 20, 77,
77, 78, 20,
20, 21, 78,
78, 79, 21,
21, 22, 79,
79, 80, 22,
22, 23, 80,
80, 81, 23,
23, 24, 81,
81, 82, 24,
24, 25, 82,
82, 83, 25,
25, 26, 83,
83, 84, 26,
26, 27, 84,
84, 85, 27,
27, 28, 85,
85, 86, 28,
28, 29, 86,
86, 87, 29,
29, 30, 87,
87, 88, 30,
30, 31, 88,
88, 89, 31,
31, 32, 89,
89, 90, 32,
32, 33, 90,
90, 91, 33,
33, 34, 91,
91, 92, 34,
34, 35, 92,
92, 93, 35,
35, 36, 93,
93, 94, 36,
36, 37, 94,
94, 95, 37,
37, 38, 95,
95, 96, 38,
38, 39, 96,
96, 97, 39,
39, 40, 97,
97, 98, 40,
40, 41, 98,
98, 99, 41,
41, 42, 99,
99, 100, 42,
42, 43, 100,
100, 101, 43,
43, 44, 101,
101, 102, 44,
44, 45, 102,
102, 103, 45,
45, 46, 103,
103, 104, 46,
46, 47, 104,
104, 105, 47,
47, 48, 105,
105, 106, 48,
48, 49, 106,
106, 107, 49,
49, 50, 107,
107, 108, 50,
50, 51, 108,
108, 109, 51,
51, 52, 109,
109, 110, 52,
52, 53, 110,
110, 111, 53,
53, 54, 111,
111, 112, 54,
54, 55, 112,
112, 113, 55,
55, 56, 113,
113, 114, 56,
56, 57, 114,
114, 115, 57,
57, 1, 59,
57, 59, 115,
    };

    unsigned int VBO_cyl, VAO_cyl, EBO_cyl;
    glGenVertexArrays(1, &VAO_cyl);
    glGenBuffers(1, &VBO_cyl);
    glGenBuffers(1, &EBO_cyl);

    glBindVertexArray(VAO_cyl);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cyl);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_vertices), cylinder_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cyl);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinder_indices), cylinder_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    unsigned int VBO_cone, VAO_cone, EBO_cone;
    glGenVertexArrays(1, &VAO_cone);
    glGenBuffers(1, &VBO_cone);
    glGenBuffers(1, &EBO_cone);

    glBindVertexArray(VAO_cone);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cone);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_vertices), cylinder_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cone);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cone_indices), cone_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO1;
    glGenVertexArrays(1, &lightCubeVAO1);
    glBindVertexArray(lightCubeVAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cyl);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cyl);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int lightCubeVAO2;
    glGenVertexArrays(1, &lightCubeVAO2);
    glBindVertexArray(lightCubeVAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cone);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cone);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath;
    unsigned int diffMap;
    string specularMapPath = "container2_specular.png";
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    diffuseMapPath = "wall.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_wall = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "floor_.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_floor = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "box.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_box = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "bottol.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bottol_box = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "floor_.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    // Create a Cone2 object
    Cone2 cone_chair = Cone2(
        1.0f,                   // Radius
        2.0f,                   // Height
        36,                     // Sector count (smoothness of the cone base)
        glm::vec3(0.2f, 0.2f, 0.2f), // Ambient color
        glm::vec3(1.0f, 0.5f, 0.3f), // Diffuse color
        glm::vec3(1.0f, 1.0f, 1.0f), // Specular color
        32.0f,                 // Shininess
        diffMap,            // Diffuse texture ID
        specMap,           // Specular texture ID
        0.0f, 0.0f,            // Texture coordinates (Xmin, Ymin)
        1.0f, 1.0f             // Texture coordinates (Xmax, Ymax)
    );

    diffuseMapPath = "mirror.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    CylinderWithTexture cylinder_window = CylinderWithTexture(0.5f, 0.5f, 1.5f, 36, 2,
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        16.0f,
        diffMap, // Assume 1 is a valid texture ID for the diffuse map
        specMap // Assume 2 is a valid texture ID for the specular map
    );

    diffuseMapPath = "besin.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_besin = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "silver3.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    CylinderWithTexture cylinder_design1 = CylinderWithTexture(0.5f, 0.5f, 1.5f, 36, 2,
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        16.0f,
        diffMap, // Assume 1 is a valid texture ID for the diffuse map
        specMap // Assume 2 is a valid texture ID for the specular map
    );

    diffuseMapPath = "golden.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    CylinderWithTexture cylinder_design2 = CylinderWithTexture(0.5f, 0.5f, 1.5f, 36, 2,
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        16.0f,
        diffMap, // Assume 1 is a valid texture ID for the diffuse map
        specMap // Assume 2 is a valid texture ID for the specular map
    );
    diffuseMapPath = "bronze.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    CylinderWithTexture cylinder_design3 = CylinderWithTexture(0.5f, 0.5f, 1.5f, 36, 2,
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        16.0f,
        diffMap, // Assume 1 is a valid texture ID for the diffuse map
        specMap // Assume 2 is a valid texture ID for the specular map
    );

    diffuseMapPath = "golden2.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    CylinderWithTexture cylinder_design4 = CylinderWithTexture(0.5f, 0.5f, 1.5f, 36, 2,
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        16.0f,
        diffMap, // Assume 1 is a valid texture ID for the diffuse map
        specMap // Assume 2 is a valid texture ID for the specular map
    );

    diffuseMapPath = "blue.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    CylinderWithTexture cylinder_design5 = CylinderWithTexture(0.5f, 0.5f, 1.5f, 36, 2,
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        16.0f,
        diffMap, // Assume 1 is a valid texture ID for the diffuse map
        specMap // Assume 2 is a valid texture ID for the specular map
    );

    diffuseMapPath = "hexagon.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Hexagon hexagon_design1(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "hexagon2.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Hexagon hexagon_design2(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "hexagon3.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Hexagon hexagon_design3(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "sofa.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_sofa = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        lightingShader.setVec3("diectionalLight.directiaon", 0.0f, 3.0f, 20.0f);
        lightingShader.setVec3("diectionalLight.ambient", .2, .2, .2);
        lightingShader.setVec3("diectionalLight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("diectionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setBool("dlighton", directionalLightOn);


        lightingShader.setVec3("spotlight.position", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotlight.direction", 0, -1, 0);
        lightingShader.setVec3("spotlight.ambient", .2, .2, .2);
        lightingShader.setVec3("spotlight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotlight.k_c", 1.0f);
        lightingShader.setFloat("spotlight.k_l", 0.09);
        lightingShader.setFloat("spotlight.k_q", 0.032);
        lightingShader.setFloat("cos_theta", glm::cos(glm::radians(5.5f)));
        lightingShader.setBool("spotlighton", spotLightOn);


        //pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
        //    1.0f, 1.0f, 1.0f,     // ambient
        //    1.0f, 1.0f, 1.0f,      // diffuse
        //    1.0f, 1.0f, 1.0f,        // specular
        //    1.0f,   //k_c
        //    0.09f,  //k_l
        //    0.032f, //k_q
        //    1,       // light number
        //    glm::cos(glm::radians(20.5f)),
        //    glm::cos(glm::radians(25.0f)),
        //    0, -1, 0
        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, globalTranslationMatrix;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        globalTranslationMatrix = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", globalTranslationMatrix);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        // glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);


        // ************************************************************************ Boundary ************************************************************************

        // Drink Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.0f, 7.5f, 0.5f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //Design Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.0f, 30.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.5f, 7.5f, 0.5f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //Besin Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.5f, 7.5f, 30.0f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.5f, -0.5f, 30.5f));
        model = globalTranslationMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

        

        // ************************************************************************ Box ************************************************************************

        for (int i = 0; i < 6; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + i * 3, 0.0f, 0.5f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 2.0f, 2.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_box.drawCubeWithTexture(lightingShaderWithTexture, model);
        }


        // ************************************************************************ Chair ************************************************************************

        for (int i = 0; i < 6; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f + i * 2.7f, 0.0f, 4.5f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
            model = globalTranslationMatrix * scaleMatrix;
            cone_chair.drawConeWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f + i * 2.7f, 1.7f, 4.5f));
            glm::mat4 rotateMatrix = glm::rotate(translateMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
            model = globalTranslationMatrix * scaleMatrix;
            cone_chair.drawConeWithTexture(lightingShaderWithTexture, model);
        }

        // ************************************************************************ Besin ************************************************************************

        glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 5.0f);

        for (int i = 0; i < 4; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 4.8f, 6.5f + i * 3));
            glm::mat4 rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 rotateMatrix2 = glm::rotate(rotateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(rotateMatrix2, glm::vec3(3.5f, 0.1f, 3.5f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_window.drawCylinder(lightingShaderWithTexture, model, viewPos);
        }
        for (int i = 0; i < 4; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 5.0f + i * 3));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 2.0f, 3.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_besin.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        // ************************************************************************ Design ************************************************************************

        glm::mat4 rotateMatrix;

        for (int i = 0; i < 3; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f + 5 * i, 5.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.3f, 0.1f, 1.3f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design3.drawCylinder(lightingShaderWithTexture, model, viewPos);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(6.5f + 5 * i, 6.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.1f, 0.1f, 1.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design2.drawCylinder(lightingShaderWithTexture, model, viewPos);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(7.8f + 5 * i, 5.2f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.9f, 0.1f, 0.9f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design1.drawCylinder(lightingShaderWithTexture, model, viewPos);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(7.3f + 5 * i, 4.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.7f, 0.1f, 0.7f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design4.drawCylinder(lightingShaderWithTexture, model, viewPos);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f + 5 * i, 3.8f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.5f, 0.1f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design5.drawCylinder(lightingShaderWithTexture, model, viewPos);
        }

        // ************************************************************************ Design 2 ************************************************************************


        for (int i = 0; i < 3; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f + 5 * i, 5.0f, 0.5f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.3f, 0.1f, 1.3f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design3.drawCylinder(lightingShaderWithTexture, model, viewPos);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(6.5f + 5 * i, 6.0f, 0.5f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.1f, 0.1f, 1.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design2.drawCylinder(lightingShaderWithTexture, model, viewPos);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(7.8f + 5 * i, 5.2f, 0.5f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.9f, 0.1f, 0.9f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design1.drawCylinder(lightingShaderWithTexture, model, viewPos);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(7.3f + 5 * i, 4.0f, 0.5f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.7f, 0.1f, 0.7f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design4.drawCylinder(lightingShaderWithTexture, model, viewPos);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f + 5 * i, 3.8f, 0.5f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.5f, 0.1f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design5.drawCylinder(lightingShaderWithTexture, model, viewPos);
        }


        // ************************************************************************ Hexagon ************************************************************************
        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 4.0f, 25.0f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.8f, 1.8f, 1.8f));
        model = globalTranslationMatrix * scaleMatrix;
        hexagon_design1.drawHexagonWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 2.6f, 22.25f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
        model = globalTranslationMatrix * scaleMatrix;
        hexagon_design2.drawHexagonWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 5.15f, 22.4f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
        model = globalTranslationMatrix * scaleMatrix;
        hexagon_design3.drawHexagonWithTexture(lightingShaderWithTexture, model);

        // ************************************************************************ Chair ************************************************************************

        //1st set
        
        //chair
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(1.0f, 0.0f, 13.0 + i*4.4f);  // Translation for the chair
            glm::vec3 rotation(0.0f, 90.0f, 0.0f);  // Rotation for the chair (pitch, yaw, roll in degrees)
            drawChairWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, ourShader, cylinder_window, cube_floor, viewPos);
        }

        //table
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(1.5f, 0.0f, 3.1f+i*4.4f);  // Translation for the table
            glm::vec3 rotation(0.0f, 0.0f, 0.0f);  // Rotation for the table (pitch, yaw, roll in degrees)
            drawTableWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, ourShader, cube_floor);
        }

        //chair
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(18.0f, 0.0f, 6.0 + i * 4.4f);  // Translation for the chair
            glm::vec3 rotation(0.0f,-90.0f, 0.0f);  // Rotation for the chair (pitch, yaw, roll in degrees)
            drawChairWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, ourShader, cylinder_window, cube_floor, viewPos);
        }


        //2nd set

        //chair
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(8.0f, 0.0f, 13.0 + i * 4.4f);  // Translation for the chair
            glm::vec3 rotation(0.0f, 90.0f, 0.0f);  // Rotation for the chair (pitch, yaw, roll in degrees)
            drawChairWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, ourShader, cylinder_window, cube_floor, viewPos);
        }

        //table
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(8.5f, 0.0f, 3.1f + i * 4.4f);  // Translation for the table
            glm::vec3 rotation(0.0f, 0.0f, 0.0f);  // Rotation for the table (pitch, yaw, roll in degrees)
            drawTableWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, ourShader, cube_floor);
        }

        //chair
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(25.0, 0.0f, 6.0 + i * 4.4f);  // Translation for the chair
            glm::vec3 rotation(0.0f, -90.0f, 0.0f);  // Rotation for the chair (pitch, yaw, roll in degrees)
            drawChairWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, ourShader, cylinder_window, cube_floor, viewPos);
        }

        //sofa
        for (int i = 0; i < 3; i++) {
            glm::vec3 sofaTranslation(-8.0f + i*5.5f, 0.0f, 22.5f);  // Translation for the sofa
            glm::vec3 sofaRotation(0.0f, 0.0f, 0.0f);  // Rotation for the sofa (pitch, yaw, roll in degrees)
            drawSofaWithTransformations(identityMatrix, globalTranslationMatrix, sofaTranslation, sofaRotation, lightingShaderWithTexture, ourShader, cube_floor, cube_sofa);
        }
        // ************************************************************************************************************************************************

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // ************************************************************************ Light ************************************************************************

        // ************************************************************************ Wall Light ************************************************************************

        for (int i = 0; i < 3; i++) {
            glBindVertexArray(lightCubeVAO1);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f + 5*i, 5.2f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.0f, -0.15f, 1.0f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        for (int i = 0; i < 3; i++) {
            glBindVertexArray(lightCubeVAO1);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f + 5 * i, 5.2f, 0.75f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.0f, -0.15f, 1.0f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        // ************************************************************************ Room Corner Light ************************************************************************

        for (int i = 0; i < 2; i++) {
            glBindVertexArray(lightCubeVAO);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 29.75f - i*29.25f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.5f, 15.0f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        for (int i = 0; i < 2; i++) {
            glBindVertexArray(lightCubeVAO);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 29.75f - i * 29.25f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.5f, 15.0f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        for (int i = 0; i < 2; i++) {
            glBindVertexArray(lightCubeVAO);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 7.5f, 29.75f - i * 29.25f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(46.0f, -0.5f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        // ************************************************************************ Table Light ************************************************************************

        for (int i = 0; i < 6; i++) {
            glBindVertexArray(lightCubeVAO1);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + i * 4.0f, 5.0f, 5.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 1.7, 1.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        for (int i = 0; i < 6; i++) {
            glBindVertexArray(lightCubeVAO1);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + i * 4.0f, 5.0f, 15.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 1.7, 1.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        for (int i = 0; i < 6; i++) {
            glBindVertexArray(lightCubeVAO1);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + i * 4.0f, 5.0f, 25.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 1.7, 1.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO1);
    glDeleteVertexArrays(1, &lightCubeVAO2);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(r, g, b));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) translate_Y += 0.1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) translate_Y -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) translate_X -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) translate_X += 0.1;
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) translate_Z += 0.1;
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) translate_Z -= 0.1;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        rotateAngle_X -= 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        rotateAngle_Y -= 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        rotateAngle_Z -= 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        pointlight1.turnOff();
        pointlight2.turnOff();
        pointlight3.turnOff();
        pointlight4.turnOff();
        directionalLightOn = false;
        spotLightOn = false;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        pointlight1.turnOn();
        pointlight2.turnOn();
        pointlight3.turnOn();
        pointlight4.turnOn();
        directionalLightOn = true;
        spotLightOn = true;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        pointlight1.turnOff();
        pointlight2.turnOff();
        pointlight3.turnOff();
        pointlight4.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        pointlight1.turnOn();
        pointlight2.turnOn();
        pointlight3.turnOn();
        pointlight4.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        pointlight1.turnOn();
        pointlight3.turnOff();
        pointlight2.turnOff();
        pointlight4.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        directionalLightOn = false;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        directionalLightOn = true;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        spotLightOn = false;
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        spotLightOn = true;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
    {
        pointlight1.turnAmbientOff();
        pointlight2.turnAmbientOff();
        pointlight3.turnAmbientOff();
        pointlight4.turnAmbientOff();
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
    {
        pointlight1.turnAmbientOn();
        pointlight2.turnAmbientOn();
        pointlight3.turnAmbientOn();
        pointlight4.turnAmbientOn();
    }
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
    {
        pointlight1.turnDiffuseOff();
        pointlight2.turnDiffuseOff();
        pointlight3.turnDiffuseOff();
        pointlight4.turnDiffuseOff();
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
    {
        pointlight1.turnDiffuseOn();
        pointlight2.turnDiffuseOn();
        pointlight3.turnDiffuseOn();
        pointlight4.turnDiffuseOn();
    }
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
    {
        pointlight1.turnSpecularOff();
        pointlight2.turnSpecularOff();
        pointlight3.turnSpecularOff();
        pointlight4.turnSpecularOff();
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
    {
        pointlight1.turnSpecularOn();
        pointlight2.turnSpecularOn();
        pointlight3.turnSpecularOn();
        pointlight4.turnSpecularOn();
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        fanOn = true;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        fanOn = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    // camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

glm::mat4 RotationMatricesX(float theta) {
    // Calculate cosine and sine of the angle
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    // Rotation matrix around X-axis
    return glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosTheta, -sinTheta, 0.0f,
        0.0f, sinTheta, cosTheta, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

glm::mat4 RotationMatricesY(float theta) {
    // Calculate cosine and sine of the angle
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    // Rotation matrix around Y-axis
    return glm::mat4(
        cosTheta, 0.0f, sinTheta, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sinTheta, 0.0f, cosTheta, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

glm::mat4 RotationMatricesZ(float theta) {
    // Calculate cosine and sine of the angle
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    // Rotation matrix around Z-axis
    return glm::mat4(
        cosTheta, -sinTheta, 0.0f, 0.0f,
        sinTheta, cosTheta, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}