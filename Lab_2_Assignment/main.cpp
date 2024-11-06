#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;
float rotateAngle = 0.0f;
float translate_X = 0.0f;
float translate_Y = 0.0f;
float scale_X = 1.0f;
float scale_Y = 1.0f;

// Additional variables for independent movement of other elements
float otherRotateAngle = 0.0f;
float otherTranslate_X = 0.0f;
float otherTranslate_Y = 0.0f;
float otherScale_X = 1.0f;
float otherScale_Y = 1.0f;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangle with Half Circle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define vertices for the triangle and half-circle
    std::vector<float> vertices = {
        // Triangle vertices
        -0.13f, 0.0f, 0.0f, // left
         0.13f, 0.0f, 0.0f, // right
         0.0f, -0.5f, 0.0f  // top
    };

    // Generate vertices for the full circle
    int numSegments = 40;
    float radius = 0.15f; // Radius of the circle (8 cm diameter)

    vertices.push_back(0.0f); // Center vertex for the full circle
    vertices.push_back(0.0f); // y-coordinate at the center of the circle
    vertices.push_back(0.0f);

    // Generate vertices for the full circle (0 to 2π radians)
    for (int i = 0; i <= numSegments; ++i) {
        float theta = 2.0f * glm::pi<float>() * (float(i) / numSegments); // Angle from 0 to 2π
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Apply translation, rotation, and scaling for each object (triangle and circles)

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Independent transformations for the orange triangle
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translationMatrixTriangle = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
        glm::mat4 rotationMatrixTriangle = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scaleMatrixTriangle = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
        glm::mat4 modelMatrixTriangle = translationMatrixTriangle * rotationMatrixTriangle * scaleMatrixTriangle;

        // Set shader program and pass transformation matrix for the orange triangle
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrixTriangle));

        // Set color for the triangle (orange) and draw
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
        glUniform4f(colorLoc, 1.0f, 0.5f, 0.2f, 1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Unified transformation for other elements
        glm::mat4 unifiedTranslation = glm::translate(identityMatrix, glm::vec3(otherTranslate_X, otherTranslate_Y, 0.0f));
        glm::mat4 unifiedRotation = glm::rotate(identityMatrix, glm::radians(otherRotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 unifiedScaling = glm::scale(identityMatrix, glm::vec3(otherScale_X, otherScale_Y, 1.0f));
        glm::mat4 unifiedModelMatrix = unifiedTranslation * unifiedRotation * unifiedScaling;

        // Draw circles with unified transformations
        std::vector<glm::vec3> circleTranslations = {
            {0.0f, 0.0f, 0.0f}, {-0.02f, 0.17f, 0.0f}, {0.02f, 0.34f, 0.0f}, {-0.05f, 0.46f, 0.0f}
        };
        std::vector<glm::vec4> circleColors = {
            {0.53f, 0.81f, 0.98f, 1.0f}, {1.0f, 0.0f, 0.5f, 1.0f}, {0.56f, 0.93f, 0.56f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}
        };

        for (int i = 0; i < 4; ++i) {
            glm::mat4 translationMatrix = glm::translate(unifiedModelMatrix, circleTranslations[i]);
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix));
            glUniform4f(colorLoc, circleColors[i].r, circleColors[i].g, circleColors[i].b, circleColors[i].a);

            if (i == 3) { // Red circle, apply scaling
                glm::mat4 scaleRed = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
                translationMatrix = translationMatrix * scaleRed;
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix));
            }
            glDrawArrays(GL_TRIANGLE_FAN, 3, numSegments + 2);
        }

        // Apply unified transformations for the black triangle
        glm::mat4 blackTriangleBaseTransform = unifiedModelMatrix;

        // Small black triangle with unified transformation and additional specific transformations
        glm::mat4 smallTriangleTransform = glm::translate(glm::scale(blackTriangleBaseTransform, glm::vec3(0.02f, 0.3f, 1.0f)), glm::vec3(-3.0f, 1.7f, 1.0f));
        smallTriangleTransform = glm::rotate(smallTriangleTransform, glm::radians(-100.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Set color and transformation matrix for the black triangle
        glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(smallTriangleTransform));
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Rotation by 0.1 degrees when the "R" key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        otherRotateAngle += 0.1f;
        if (otherRotateAngle >= 360.0f)
        {
            otherRotateAngle = 0.0f; // Reset angle to avoid overflow
        }
    }

    // Rotation by 0.1 degrees when the "T" key is pressed
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        rotateAngle += 0.1f;
        if (rotateAngle >= 360.0f)
        {
            rotateAngle = 0.0f; // Reset angle to avoid overflow
        }
    }

    // Scaling using + and - keys
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) scale_X += 0.01f, scale_Y += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) scale_X -= 0.01f, scale_Y -= 0.01f;

    // Scaling using numpad '+' and '-'
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) otherScale_X += 0.01f, otherScale_Y += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) otherScale_X -= 0.01f, otherScale_Y -= 0.01f;

    // Translation using numpad keys
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) translate_Y += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) translate_Y -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) translate_X -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) translate_X += 0.01f;

    // Independent transformations for other elements
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) otherTranslate_Y += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) otherTranslate_Y -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) otherTranslate_X -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) otherTranslate_X += 0.01f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
