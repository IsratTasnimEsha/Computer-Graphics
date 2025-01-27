//
//  cone.h
//  test
//
//  Created by [Your Name] on [Date].
//

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

# define PI 3.1416

class Cone2 {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;
    float shininess;

    Cone2(float radius, float height, int sectorCount, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny,
        unsigned int dMap, unsigned int sMap, float textureXmin, float textureYmin, float textureXmax, float textureYmax) {
        set(radius, height, sectorCount, amb, diff, spec, shiny, dMap, sMap, textureXmin, textureYmin, textureXmax, textureYmax);
        setUpConeVertexDataAndConfigureVertexAttribute();
    }
    ~Cone2() {}

    void set(float radius, float height, int sectorCount, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny,
        unsigned int dMap, unsigned int sMap, float textureXmin, float textureYmin, float textureXmax, float textureYmax) {
        this->radius = radius;
        this->height = height;
        this->sectorCount = sectorCount;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->TXmin = textureXmin;
        this->TXmax = textureXmax;
        this->TYmin = textureYmin;
        this->TYmax = textureYmax;

        buildCoordinatesAndIndices();
    }

    void drawConeWithTexture(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();
        shader.setVec3("material.ambient", this->ambient);
        shader.setVec3("material.diffuse", this->diffuse);
        shader.setVec3("material.specular", this->specular);
        shader.setFloat("material.shininess", this->shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        shader.setMat4("model", model);

        glBindVertexArray(coneVAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int coneVAO, coneVBO, coneEBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float radius, height;
    int sectorCount;

    void buildCoordinatesAndIndices() {
        float sectorStep = 2 * PI / sectorCount;
        float sectorAngle;

        // Bottom center point
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
        vertices.push_back(0.5f);
        vertices.push_back(0.5f);

        // Bottom circle vertices
        for (int i = 0; i <= sectorCount; ++i) {
            sectorAngle = i * sectorStep;
            float x = radius * cosf(sectorAngle);
            float z = radius * sinf(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(0.0f);
            vertices.push_back(z);
            vertices.push_back(0.0f);
            vertices.push_back(-1.0f);
            vertices.push_back((cosf(sectorAngle) + 1) * 0.5f);
            vertices.push_back((sinf(sectorAngle) + 1) * 0.5f);
        }

        // Apex point
        vertices.push_back(0.0f);
        vertices.push_back(height);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.5f);
        vertices.push_back(1.0f);

        // Indices for bottom circle
        for (int i = 1; i <= sectorCount; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        // Indices for side triangles
        int apexIndex = sectorCount + 2;
        for (int i = 1; i <= sectorCount; ++i) {
            indices.push_back(i);
            indices.push_back(apexIndex);
            indices.push_back(i + 1);
        }
    }

    void setUpConeVertexDataAndConfigureVertexAttribute() {
        glGenVertexArrays(1, &coneVAO);
        glGenBuffers(1, &coneVBO);
        glGenBuffers(1, &coneEBO);

        glBindVertexArray(coneVAO);

        glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coneEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
};
#pragma once
#pragma once
