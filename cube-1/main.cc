#include <common-main.hh>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

GLuint arrayBufferTriangle;
GLuint elementArrayBufferTriangle;
GLuint vertexArrayObjectTriangle;

GLuint shaderProgram;

glm::vec3 cameraPos = glm::vec3(1.2, 1.2, 1.2);
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
    // .. and an index buffer
    glGenBuffers(1, &elementArrayBufferTriangle);

    // upload vertex data
    Vertex vertices[8] = {
            {glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0)},
            {glm::vec3(-1, -1, +1), glm::vec3(0, 0, 1)},
            {glm::vec3(+1, -1, -1), glm::vec3(1, 0, 0)},
            {glm::vec3(+1, -1, +1), glm::vec3(1, 0, 1)},
            {glm::vec3(-1, +1, -1), glm::vec3(0, 1, 0)},
            {glm::vec3(-1, +1, +1), glm::vec3(0, 1, 1)},
            {glm::vec3(+1, +1, -1), glm::vec3(1, 1, 0)},
            {glm::vec3(+1, +1, +1), glm::vec3(1, 1, 1)}
    };
    uint16_t indices[3 * 12] = {
            // bottom
            0, 2, 1,
            1, 2, 3,

            // top
            4, 5, 6,
            5, 7, 6,

            // front
            0, 4, 2,
            4, 6, 2,

            // back
            1, 3, 5,
            5, 3, 7,

            // left
            0, 1, 4,
            1, 5, 4,

            // right
            2, 6, 3,
            6, 7, 3,
    };
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // "unbind"

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3 * 12, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // generate VAO descriptor
    glGenVertexArrays(1, &vertexArrayObjectTriangle);
    // configure vertex array object (VAO)
    glBindVertexArray(vertexArrayObjectTriangle);

    // location 0: position as vec3
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

    // location 1: color as vec3
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));

    // bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);

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

    glm::mat4 modelMatrix = glm::rotate(runtime* 0, glm::vec3(0, 1, 0)); // rotation
    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float)windowHeight, 0.05f, 20.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);
    glm::mat4 modelViewProj = projMatrix * viewMatrix * modelMatrix;

    // use shader
    glUseProgram(shaderProgram);

    // set time uniform
    glUniform1f(glGetUniformLocation(shaderProgram, "uRuntime"), runtime);
    // set camera matrix
    GLint loc = glGetUniformLocation(shaderProgram, "uModelViewProjection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelViewProj));

    // bind VAO
    glBindVertexArray(vertexArrayObjectTriangle);
    // draw the first 3x12 indices as a triangle list
    glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_SHORT, 0);
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // nothing special
}

// Add main() and helper implementations
#include <common-main.cc>
