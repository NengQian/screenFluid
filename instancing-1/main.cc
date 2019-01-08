#include <common-main.hh>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex;
};

GLuint arrayBufferTriangle;
GLuint elementArrayBufferTriangle;
GLuint vertexArrayObjectTriangle;
GLuint texName;
GLuint texBufferName;

GLuint shaderProgram;

glm::vec3 cameraPos = glm::vec3(1.5, 1.5, 1.2);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);

int mode = 1;

void initResources() {
    std::cout << "init resources " << windowWidth << " x " << windowHeight << std::endl;

    // ============================
    // create shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // setup shader source
    std::string codeVS = readFile("vertexShader.glsl");
    std::string codeFS = readFile("fragmentShader.glsl");
    const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
    glShaderSource(vertexShader, 1, &cVS, nullptr);
    glShaderSource(fragmentShader, 1, &cFS, nullptr);
    // compile shader
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    checkShader(vertexShader);
    checkShader(fragmentShader);

    // create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // delete unused shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ============================
    // Create a texture and a texture buffer
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_BUFFER, texName);

    glGenBuffers(1, &texBufferName);
    glBindBuffer(GL_TEXTURE_BUFFER, texBufferName);

    // gen data (pos + color)
    std::vector<glm::vec3> instances;
    instances.resize(16 * 16 * 16 * 2);

    srand(123456);
    for (int i = 0; i < 16 * 16 * 16; ++i) {
        int faction = rand() % 3;

        glm::vec3 pos, color;
        switch (faction) {
            case 0:
                color = glm::vec3(1, 0, 0);
                pos = glm::vec3(
                        (rand() % 10000) / 5000.0 - 1,
                        -1,
                        (rand() % 10000) / 5000.0 - 1
                );
                break;
            case 1:
                color = glm::vec3(0, 1, 0);
                pos = glm::vec3(0, (rand() % 10000) / 5000.0 - 1, 0);
                break;
            default:
            case 2:
                color = glm::vec3(0, 0, 1);
                float t = (rand() % 10000) / 5000.0f * 6.28f;
                pos = glm::vec3(glm::cos(t), 1, glm::sin(t));
                break;
        }

        instances[i * 2 + 0] = pos + (glm::vec3(rand() % 1000, rand() % 1000, rand() % 1000) / 500.0f - 1.0f) * 0.15f;
        instances[i * 2 + 1] = color;
    }

    // upload data
    glBufferData(GL_TEXTURE_BUFFER, sizeof(instances[0]) * instances.size(), instances.data(), GL_STATIC_DRAW);
    // couple buffer and texture
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, texBufferName);

    // unbind
    glBindTexture(GL_TEXTURE_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);


    // ============================
    // generate a GPU buffer for our triangle
    glGenBuffers(1, &arrayBufferTriangle);
    // .. and an index buffer
    glGenBuffers(1, &elementArrayBufferTriangle);

    // upload vertex data
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    for (int i = 0; i < 3; ++i) {
        for (int s = 0; s < 2; ++s) {
            glm::vec3 n{
                    i == 0,
                    i == 1,
                    i == 2
            };
            n *= s * 2 - 1;

            auto mid = n;
            auto left = glm::abs(dot(n, glm::vec3(0, 1, 0))) < .2 ?
                        cross(glm::vec3(0, 1, 0), n) :
                        cross(glm::vec3(0, 0, 1), n);
            auto top = cross(left, n);

            auto baseIndex = vertices.size();

            vertices.push_back({mid - left - top, n, {0, 0}});
            vertices.push_back({mid - left + top, n, {0, 1}});
            vertices.push_back({mid + left - top, n, {1, 0}});
            vertices.push_back({mid + left + top, n, {1, 1}});

            indices.push_back((uint16_t) (baseIndex + 0));
            indices.push_back((uint16_t) (baseIndex + 1));
            indices.push_back((uint16_t) (baseIndex + 2));

            indices.push_back((uint16_t) (baseIndex + 1));
            indices.push_back((uint16_t) (baseIndex + 3));
            indices.push_back((uint16_t) (baseIndex + 2));
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // generate VAO descriptor
    glGenVertexArrays(1, &vertexArrayObjectTriangle);
    // configure vertex array object (VAO)
    glBindVertexArray(vertexArrayObjectTriangle);

    // location 0: position as vec3
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

    // location 1: normal as vec3
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

    // location 2: tex coord as vec2
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tex));

    // bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);

    glBindVertexArray(0); // "unbind" VAO

    // ============================
    // Enable zBuffering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // ============================
    // Enable Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void deleteResources() {
    // proper cleanup of resources
    glDeleteVertexArrays(1, &vertexArrayObjectTriangle);
    glDeleteBuffers(1, &arrayBufferTriangle);
    glDeleteTextures(1, &texName);

    glUseProgram(0);
    glDeleteProgram(shaderProgram);
}

void draw() {
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.00, 0.33, 0.62, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 0.05f, 20.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);
    glm::mat4 viewProj = projMatrix * viewMatrix;

    // use shader
    glUseProgram(shaderProgram);

    // bind texture buffer to unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, texName);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexBuffer"), 0);

    // set light information
    glm::vec3 lightPos = glm::vec3(glm::cos(runtime) * 8, 5, glm::sin(runtime) * 8);
    glUniform3fv(glGetUniformLocation(shaderProgram, "uLightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "uCamPos"), 1, glm::value_ptr(cameraPos));
    // set time uniform
    glUniform1f(glGetUniformLocation(shaderProgram, "uRuntime"), runtime);
    // set mode uniform
    glUniform1i(glGetUniformLocation(shaderProgram, "uMode"), mode);
    // set camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewProjection"), 1, GL_FALSE,
                       glm::value_ptr(viewProj));

    // bind VAO
    glBindVertexArray(vertexArrayObjectTriangle);
    // draw the first 3x12 indices as a triangle list
    glDrawElementsInstanced(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_SHORT, 0, 16 * 16 * 16);
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // nothing special

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        mode = 1;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        mode = 2;
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        mode = 3;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        mode = 4;
}

// Add main() and helper implementations
#include <common-main.cc>
