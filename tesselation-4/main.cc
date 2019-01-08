#include <common-main.hh>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

GLuint arrayBufferPositions;
GLuint arrayBufferNormals;
GLuint elementArrayBufferTriangle;
GLuint vertexArrayObjectTriangle;

GLuint texName[3];

GLuint shaderProgram;

int mode = 1;

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
    // generate a dummy VAO descriptor
    glGenVertexArrays(1, &vertexArrayObjectTriangle);

    // ============================
    // Create textures
    // https://www.filterforge.com/filters/6903.html
    glGenTextures(3, texName);
    for (int i = 0; i < 3; ++i) {
        glBindTexture(GL_TEXTURE_2D, texName[i]);
        Texture tex;
        switch (i) {
            case 0:
                readPPM("../res/cobble.color.ppm", tex);
                break;
            case 1:
                readPPM("../res/cobble.normal.ppm", tex);
                break;
            default:
            case 2:
                readPPM("../res/cobble.height.ppm", tex);
                break;
        }
        glTexImage2D(GL_TEXTURE_2D,    // texture target
                     0,                // level (0 = finest)
                     GL_RGB,           // internal format (aka GPU format)
                     tex.width,        // width in px
                     tex.height,       // height in px
                     0,                // border (deprecated)
                     GL_RGB,           // CPU data arrangement
                     GL_UNSIGNED_BYTE, // CPU data type
                     tex.colors.data() // actual data
        );
        // setup parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, 0); // unbind to prevent state hickups

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
    glDeleteBuffers(1, &arrayBufferPositions);
    glDeleteBuffers(1, &arrayBufferNormals);

    glUseProgram(0);
    glDeleteProgram(shaderProgram);
}

void draw() {
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.00, 0.33, 0.62, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 modelMatrix = glm::mat4(); // rotation
    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 0.05f, 20.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);

    // use shader
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uMode"), mode);

    // bind texture to unit 0-2
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexColor"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texName[1]);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexNormal"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texName[2]);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexHeight"), 2);

    // set time uniform
    glUniform1f(glGetUniformLocation(shaderProgram, "uRuntime"), runtime);
    // set camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::vec3 lightPos = glm::vec3(3, 5, 1);
    glUniform3fv(glGetUniformLocation(shaderProgram, "uLightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "uCamPos"), 1, glm::value_ptr(cameraPos));

    // bind VAO
    glBindVertexArray(vertexArrayObjectTriangle);
    // draw a quad
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawArrays(GL_PATCHES, 0, 4);
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        mode = 1;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        mode = 2;
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        mode = 3;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        mode = 4;
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        mode = 5;
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        mode = 6;
}

// Add main() and helper implementations
#include <common-main.cc>
