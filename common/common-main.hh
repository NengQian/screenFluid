#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <SOIL2.h>

#define readFile(FILE) readFileDirect(resolvePath(FILE, __FILE__))
std::string readFileDirect(std::string filename);
std::string resolvePath(std::string filename, std::string relpathOrFile);
void checkShader(GLuint shaderID);
void checkProgram(GLuint programID);

int windowWidth = 800;
int windowHeight = 600;

bool paused = false;
float runtime = 0.0;

struct Texture {
    int width;
    int height;
    std::vector<uint8_t> colors;
};

struct OBJVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

#define readOBJ(FILE) readOBJDirect(resolvePath(FILE, __FILE__))
#define readImage(FILE, TEX) readImageDirect(resolvePath(FILE, __FILE__), TEX)
#define readPPM(FILE, TEX) readPPMDirect(resolvePath(FILE, __FILE__), TEX)
std::vector<OBJVertex> readOBJDirect(std::string filename);
void readImageDirect(std::string filename, Texture& tex);
void readPPMDirect(std::string file, Texture& tex);

float angleAzimuth = glm::pi<float>() * 0.22f;
float angleAltitude = glm::pi<float>() * 0.20f;
float mouseZoom = 2.5f;

glm::mat4 interactiveView(glm::vec3 *camPos, glm::vec3 *camTarget) {
    glm::vec3 pos = {
            mouseZoom * glm::cos(angleAzimuth) * glm::cos(angleAltitude),
            mouseZoom * glm::sin(angleAltitude),
            mouseZoom * glm::sin(angleAzimuth) * glm::cos(angleAltitude)
    };

    if (camTarget) *camTarget = glm::vec3(0.0);
    if (camPos) *camPos = pos;

    return glm::lookAt(pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
