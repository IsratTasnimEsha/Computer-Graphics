//
//  sphere.h
//  test
//
//  Created by Nazirul Hasan on 26/9/23.
//

#ifndef sphere_h
#define sphere_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

# define PI 3.1416

using namespace std;

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

class Sphere
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    // Constructor/Destructor
    Sphere(float radius = 1.0f, int sectorCount = 20, int stackCount = 18,
        glm::vec3 amb = glm::vec3(0.24725f, 0.2245f, 0.0645f),
        glm::vec3 diff = glm::vec3(0.34615f, 0.3143f, 0.0903f),
        glm::vec3 spec = glm::vec3(0.797357f, 0.723991f, 0.208006f),
        float shiny = 32.0f)
        : verticesStride(24)
    {
        set(radius, sectorCount, stackCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &sphereVAO);
        glBindVertexArray(sphereVAO);

        // Create VBO to copy vertex data
        unsigned int sphereVBO;
        glGenBuffers(1, &sphereVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER,
            this->getVertexSize(),
            this->getVertices(),
            GL_STATIC_DRAW);

        // Create EBO to copy index data
        unsigned int sphereEBO;
        glGenBuffers(1, &sphereEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            this->getIndexSize(),
            this->getIndices(),
            GL_STATIC_DRAW);

        // Activate attribute arrays
        glEnableVertexAttribArray(0); // Position
        glEnableVertexAttribArray(1); // Normal

        // Set attribute arrays with stride and offset
        int stride = this->getVerticesStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

        // Unbind VAO and buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    ~Sphere() {}

    // Setters
    void set(float radius, int sectors, int stacks, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->radius = max(radius, 0.0f);
        this->sectorCount = max(sectors, MIN_SECTOR_COUNT);
        this->stackCount = max(stacks, MIN_STACK_COUNT);
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // Getters for interleaved vertices
    unsigned int getVertexCount() const { return (unsigned int)coordinates.size() / 3; }
    unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
    int getVerticesStride() const { return verticesStride; }
    const float* getVertices() const { return vertices.data(); }
    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const unsigned int* getIndices() const { return indices.data(); }
    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

    // Draw function
    void drawSphere(Shader& lightingShader, glm::mat4 model) const
    {
        lightingShader.use();

        // Set material properties
        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        // Draw the sphere
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES,
            this->getIndexCount(),
            GL_UNSIGNED_INT,
            (void*)0);
        glBindVertexArray(0);
    }

private:
    // Helper functions
    void buildCoordinatesAndIndices()
    {
        float x, y, z, xz;
        float nx, ny, nz, lengthInv = 1.0f / radius;
        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;
            xz = radius * cosf(stackAngle);
            y = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                x = xz * cosf(sectorAngle);
                z = xz * sinf(sectorAngle);

                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);
            }
        }

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);

                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    void buildVertices()
    {
        size_t i, j;
        size_t count = coordinates.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);
        }
    }

    // Member variables
    unsigned int sphereVAO;
    float radius;
    int sectorCount;
    int stackCount;
    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> coordinates;
    int verticesStride;
};

#endif /* sphere_h */
