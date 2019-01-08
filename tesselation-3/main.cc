#include <common-main.hh>

#include <suzanne.cc>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

#define SCENES 2
GLuint arrayBufferPositions[SCENES];
GLuint arrayBufferNormals[SCENES];
GLuint elementArrayBufferTriangle[SCENES];
GLuint vertexArrayObjectTriangle[SCENES];
int sceneIdxCount[SCENES];
int currScene = 1;

bool enableTess = false;

GLuint shaderProgram;

glm::vec3 cameraPos = glm::vec3(1., 1., 1.);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);

void initResources() {
    std::cout << "init resources " << windowWidth << " x " << windowHeight << std::endl;

    // ============================
    // create shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    GLuint tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // setup shader source
    std::string codeVS = readFile("vertexShader.glsl");
    std::string codeTCS = readFile("tesselationControlShader.glsl");
    std::string codeTES = readFile("tesselationEvaluationShader.glsl");
    std::string codeFS = readFile("fragmentShader.glsl");
    const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str(), *cTCS = codeTCS.c_str(), *cTES = codeTES.c_str();
    glShaderSource(vertexShader, 1, &cVS, nullptr);
    glShaderSource(tessControlShader, 1, &cTCS, nullptr);
    glShaderSource(tessEvalShader, 1, &cTES, nullptr);
    glShaderSource(fragmentShader, 1, &cFS, nullptr);
    // compile shader
    glCompileShader(vertexShader);
    glCompileShader(tessControlShader);
    glCompileShader(tessEvalShader);
    glCompileShader(fragmentShader);
    checkShader(vertexShader);
    checkShader(tessControlShader);
    checkShader(tessEvalShader);
    checkShader(fragmentShader);

    // create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, tessControlShader);
    glAttachShader(shaderProgram, tessEvalShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // delete unused shader
    glDeleteShader(vertexShader);
    glDeleteShader(tessControlShader);
    glDeleteShader(tessEvalShader);
    glDeleteShader(fragmentShader);

    // ============================
    // generate a GPU buffer for our triangle
    glGenBuffers(2, arrayBufferPositions);
    glGenBuffers(2, arrayBufferNormals);
    // .. and an index buffer
    glGenBuffers(2, elementArrayBufferTriangle);
    // generate VAO descriptor
    glGenVertexArrays(2, vertexArrayObjectTriangle);

    // upload vertex data
    //   - SCENE 0: suzanne
    int scene;
    {
        scene = 0;
        suzanne_calc_normals();

        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferPositions[scene]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(suzanne_vertices), suzanne_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferNormals[scene]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(suzanne_vertices), suzanne_normals, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle[scene]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(suzanne_indices), suzanne_indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // configure vertex array object (VAO)
        glBindVertexArray(vertexArrayObjectTriangle[scene]);

        // location 0: position as vec3
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferPositions[scene]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

        // location 1: normal as vec3
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferNormals[scene]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

        // bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle[scene]);

        sceneIdxCount[scene] = sizeof(suzanne_indices) / sizeof(suzanne_indices[0]);

        glBindVertexArray(0); // "unbind" VAO
    }
    //   - SCENE 1: cube
    {
        scene = 1;

        glm::vec3 positions[8] = {
                glm::vec3(-1, -1, -1) * .8f,
                glm::vec3(-1, -1, +1) * .8f,
                glm::vec3(+1, -1, -1) * .8f,
                glm::vec3(+1, -1, +1) * .8f,
                glm::vec3(-1, +1, -1) * .8f,
                glm::vec3(-1, +1, +1) * .8f,
                glm::vec3(+1, +1, -1) * .8f,
                glm::vec3(+1, +1, +1) * .8f,
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

        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferPositions[scene]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferNormals[scene]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle[scene]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // configure vertex array object (VAO)
        glBindVertexArray(vertexArrayObjectTriangle[scene]);

        // location 0: position as vec3
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferPositions[scene]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

        // location 1: normal as vec3
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferNormals[scene]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

        // bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle[scene]);

        sceneIdxCount[scene] = sizeof(indices) / sizeof(indices[0]);

        glBindVertexArray(0); // "unbind" VAO
    }

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
    glDeleteVertexArrays(2, vertexArrayObjectTriangle);
    glDeleteBuffers(2, arrayBufferPositions);
    glDeleteBuffers(2, arrayBufferNormals);
    glDeleteBuffers(2, elementArrayBufferTriangle);

    glUseProgram(0);
    glDeleteProgram(shaderProgram);
}

void draw() {
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.00, 0.33, 0.62, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 modelMatrix = glm::rotate(runtime, glm::vec3(0, 1, 0)); // rotation
    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 0.05f, 20.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);

    // use shader
    glUseProgram(shaderProgram);

    // set time uniform
    glUniform1f(glGetUniformLocation(shaderProgram, "uRuntime"), runtime);
    // set camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(shaderProgram, "uCamPos"), 1, glm::value_ptr(cameraPos));

    glUniform1i(glGetUniformLocation(shaderProgram, "uTess"), enableTess ? 1 : 0);

    // bind VAO
    glBindVertexArray(vertexArrayObjectTriangle[currScene]);
    // draw the suzanne indexed as a triangle list
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glDrawElements(GL_PATCHES, sceneIdxCount[currScene], GL_UNSIGNED_SHORT, 0);
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        currScene = 0;

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        currScene = 1;

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        enableTess = !enableTess;
}

// Add main() and helper implementations
#include <common-main.cc>
