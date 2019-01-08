#include <common-main.hh>

struct Vertex {
    glm::vec3 position;
};

GLuint arrayBufferTriangle;
GLuint vertexArrayObjectTriangle;

GLuint shaderProgram;

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
    Vertex vertices[3] = {
            {glm::vec3(-.8, -.8, 0)},
            {glm::vec3(.8, -.8, 0)},
            {glm::vec3(0, .8, 0)}
    };
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // "unbind"

    // generate VAO descriptor
    glGenVertexArrays(1, &vertexArrayObjectTriangle);
    // configure vertex array object (VAO)
    glBindVertexArray(vertexArrayObjectTriangle);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(
            0,         // attribute location index
            3,         // size (as-in nr of components)
            GL_FLOAT,  // data type (float, int, byte, etc.)
            GL_FALSE,  // if true, integers are normalized to [0,1] or [-1,1] floats
            0,         // stride, 0 = tightly packed
            (void *) 0 // offset in array buffer
    );
    glBindVertexArray(0); // "unbind"
}

void deleteResources() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    // proper cleanup of resources
    glDeleteVertexArrays(1, &vertexArrayObjectTriangle);
    glDeleteBuffers(1, &arrayBufferTriangle);

    glDeleteProgram(shaderProgram);
}

void draw() {
    // setup viewport
    glViewport(0, 0, windowWidth, windowHeight);

    // clear screen
    glClearColor(0.00, 0.33, 0.62, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // use shader
    glUseProgram(shaderProgram);

    // bind VAO
    glBindVertexArray(vertexArrayObjectTriangle);
    // draw the first three vertices as a triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // nothing special
}

// Add main() and helper implementations
#include <common-main.cc>
