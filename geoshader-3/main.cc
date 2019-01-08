#include <common-main.hh>

#include "suzanne.cc"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

GLuint arrayBufferTriangle;
GLuint elementArrayBufferTriangle;
GLuint vertexArrayObjectTriangle;

GLuint shaderProgram;

glm::vec3 cameraPos = glm::vec3(1.5, 1.2, 1.5);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);

void initResources() {
    std::cout << "init resources " << windowWidth << " x " << windowHeight << std::endl;

    // ============================
    // create shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // setup shader source
    std::string codeVS = readFile("vertexShader.glsl");
    std::string codeGS = readFile("geometryShader.glsl");
    std::string codeFS = readFile("fragmentShader.glsl");
    const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str(), *cGS = codeGS.c_str();
    glShaderSource(vertexShader, 1, &cVS, nullptr);
    glShaderSource(geometryShader, 1, &cGS, nullptr);
    glShaderSource(fragmentShader, 1, &cFS, nullptr);
    // compile shader
    glCompileShader(vertexShader);
    glCompileShader(geometryShader);
    glCompileShader(fragmentShader);
    checkShader(vertexShader);
    checkShader(geometryShader);
    checkShader(fragmentShader);

    // create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // delete unused shader
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);

    // ============================
    // generate a GPU buffer for our triangle
    glGenBuffers(1, &arrayBufferTriangle);
    // .. and an index buffer
    glGenBuffers(1, &elementArrayBufferTriangle);

    // upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(suzanne_vertices), suzanne_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // "unbind"

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(suzanne_indices), suzanne_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // generate VAO descriptor
    glGenVertexArrays(1, &vertexArrayObjectTriangle);
    // configure vertex array object (VAO)
    glBindVertexArray(vertexArrayObjectTriangle);

    // location 0: position as vec3
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) offsetof(Vertex, position));

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

    glUseProgram(0);
    glDeleteProgram(shaderProgram);
}

void draw() {
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.00, 0.33, 0.62, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 modelMatrix = glm::mat4();

    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 0.05f, 20.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);
    glm::mat4 viewProj = projMatrix * viewMatrix;

    // use shader
    glUseProgram(shaderProgram);

    // set time uniform
    glUniform1f(glGetUniformLocation(shaderProgram, "uRuntime"), runtime);
    // set camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(viewProj));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // set light information
    glm::vec3 lightPos = glm::vec3(glm::cos(runtime) * 8, 5, glm::sin(runtime) * 8);
    glUniform3fv(glGetUniformLocation(shaderProgram, "uLightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "uCamPos"), 1, glm::value_ptr(cameraPos));

    // bind VAO
    glBindVertexArray(vertexArrayObjectTriangle);
    // draw the suzanne indexed as a triangle list
    glDrawElements(GL_TRIANGLES, sizeof(suzanne_indices) / sizeof(suzanne_indices[0]), GL_UNSIGNED_SHORT, 0);
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // nothing special
}

// Add main() and helper implementations
#include <common-main.cc>
