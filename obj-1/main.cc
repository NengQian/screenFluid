#include <common-main.hh>

GLuint arrayBufferTriangle;
GLuint vertexArrayObjectTriangle;
int vertexCnt = 0;

GLuint shaderProgram;

glm::vec3 cameraPos = glm::vec3(1.5, 1.5, 1.5);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);

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
    // generate a GPU buffer for our triangle
    glGenBuffers(1, &arrayBufferTriangle);

    // upload vertex data
    // http://www.3drender.com/challenges/
    auto vertices = readOBJ("../res/madScience.obj");
    vertexCnt = (int) vertices.size();
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // "unbind"

    // generate VAO descriptor
    glGenVertexArrays(1, &vertexArrayObjectTriangle);
    // configure vertex array object (VAO)
    glBindVertexArray(vertexArrayObjectTriangle);

    // location 0: position as vec3
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void *) offsetof(OBJVertex, position));

    // location 1: normal as vec3
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void *) offsetof(OBJVertex, normal));

    // location 2: texcoord as vec2
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void *) offsetof(OBJVertex, texcoord));

    glBindVertexArray(0); // "unbind" VAO

    // ============================
    // Enable zBuffering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
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

    glm::mat4 modelMatrix = glm::translate(glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.05f));
    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 0.05f, 100.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);
    glm::mat4 viewProj = projMatrix * viewMatrix;

    // use shader
    glUseProgram(shaderProgram);

    // set time uniform
    glUniform1f(glGetUniformLocation(shaderProgram, "uRuntime"), runtime);
    // set camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(viewProj));

    // set light information
    glm::vec3 lightPos = glm::vec3(5, 8, 5);
    glUniform3fv(glGetUniformLocation(shaderProgram, "uLightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "uCamPos"), 1, glm::value_ptr(cameraPos));

    // bind VAO
    glBindVertexArray(vertexArrayObjectTriangle);
    // draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // nothing special
}

// Add main() and helper implementations
#include <common-main.cc>
