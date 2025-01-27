#ifndef hexagon_h
#define hexagon_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Hexagon {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Hexagon()
    {
        setUpHexagonVertexDataAndConfigureVertexAttribute();
    }

    Hexagon(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpHexagonVertexDataAndConfigureVertexAttribute();
    }

    Hexagon(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpHexagonVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Hexagon()
    {
        glDeleteVertexArrays(1, &hexagonVAO);
        glDeleteVertexArrays(1, &lightHexagonVAO);
        glDeleteVertexArrays(1, &lightTexHexagonVAO);
        glDeleteBuffers(1, &hexagonVBO);
        glDeleteBuffers(1, &hexagonEBO);
    }

    void drawHexagonWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexHexagonVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawHexagonWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightHexagonVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawHexagon(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(hexagonVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int hexagonVAO;
    unsigned int lightHexagonVAO;
    unsigned int lightTexHexagonVAO;
    unsigned int hexagonVBO;
    unsigned int hexagonEBO;

    void setUpHexagonVertexDataAndConfigureVertexAttribute()
    {
        // Hexagon vertex data and buffer setup
        const float Pi = 3.14159265359f;
        const int numSides = 6;

        std::vector<float> hexagon_vertices;
        std::vector<unsigned int> hexagon_indices;

        // Center of the hexagon
        hexagon_vertices.insert(hexagon_vertices.end(), { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f * (TXmax + TXmin), 0.5f * (TYmax + TYmin) });

        // Vertices on the perimeter
        for (int i = 0; i <= numSides; ++i) {
            float angle = 2.0f * Pi * i / numSides;
            float x = cos(angle);
            float y = sin(angle);

            hexagon_vertices.insert(hexagon_vertices.end(), { x, y, 0.0f, 0.0f, 0.0f, 1.0f, TXmin + (TXmax - TXmin) * (x + 1.0f) / 2.0f, TYmin + (TYmax - TYmin) * (y + 1.0f) / 2.0f });

            if (i > 0) {
                hexagon_indices.insert(hexagon_indices.end(), { 0, static_cast<unsigned int>(i), static_cast<unsigned int>(i + 1) });
            }
        }

        glGenVertexArrays(1, &hexagonVAO);
        glGenVertexArrays(1, &lightHexagonVAO);
        glGenVertexArrays(1, &lightTexHexagonVAO);
        glGenBuffers(1, &hexagonVBO);
        glGenBuffers(1, &hexagonEBO);

        glBindVertexArray(lightTexHexagonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, hexagonVBO);
        glBufferData(GL_ARRAY_BUFFER, hexagon_vertices.size() * sizeof(float), hexagon_vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hexagonEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, hexagon_indices.size() * sizeof(unsigned int), hexagon_indices.data(), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(lightHexagonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, hexagonVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hexagonEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(hexagonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, hexagonVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hexagonEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
};

#endif /* hexagon_h */
