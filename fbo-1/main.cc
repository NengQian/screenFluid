#include <common-main.hh>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 tex;
};

GLuint arrayBufferTriangle;
GLuint elementArrayBufferTriangle;
GLuint vertexArrayObjectTriangle;
GLuint texName[2];
GLuint texCubeMap;
GLuint frameBufferObject;
GLuint gbufferColor;
GLuint gbufferDepth;

GLuint shaderProgram;
GLuint shaderProgramBG;
GLuint shaderProgramPost;

glm::vec3 cameraPos = glm::vec3(1.5, 1.5, 1.2);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);

int mode = 1;

void initResources() {
    std::cout << "init resources " << windowWidth << " x " << windowHeight << std::endl;

    // ============================
    // create shader
    { // Cube
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
        checkProgram(shaderProgram);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    { // Background
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("bg.vs.glsl");
        std::string codeFS = readFile("bg.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramBG = glCreateProgram();
        glAttachShader(shaderProgramBG, vertexShader);
        glAttachShader(shaderProgramBG, fragmentShader);
        glLinkProgram(shaderProgramBG);
        checkProgram(shaderProgramBG);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    { // Post-process
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("postprocess.vs.glsl");
        std::string codeFS = readFile("postprocess.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramPost = glCreateProgram();
        glAttachShader(shaderProgramPost, vertexShader);
        glAttachShader(shaderProgramPost, fragmentShader);
        glLinkProgram(shaderProgramPost);
        checkProgram(shaderProgramPost);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

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

            vertices.push_back({mid - left - top, n, left, {0, 0}});
            vertices.push_back({mid - left + top, n, left, {0, 1}});
            vertices.push_back({mid + left - top, n, left, {1, 0}});
            vertices.push_back({mid + left + top, n, left, {1, 1}});

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

    // location 2: tangent as vec3
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tangent));

    // location 3: tex coord as vec2
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tex));

    // bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);

    glBindVertexArray(0); // "unbind" VAO

    // ============================
    // Create textures
    // https://www.filterforge.com/filters/6903.html
    glGenTextures(3, texName);
    Texture tex;
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, texName[i]);
        switch (i) {
            case 0:
                readPPM("../res/cobble.color.ppm", tex);
                break;
            case 1:
                readPPM("../res/cobble.normal.ppm", tex);
                break;
            default:
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
    // Create CubeMap
    // http://www.humus.name/index.php?page=Textures&start=48
    glGenTextures(1, &texCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texCubeMap);

    readImage("../res/buddha/posx.jpg", tex);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.colors.data());
    readImage("../res/buddha/negx.jpg", tex);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.colors.data());

    readImage("../res/buddha/posy.jpg", tex);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.colors.data());
    readImage("../res/buddha/negy.jpg", tex);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.colors.data());

    readImage("../res/buddha/posz.jpg", tex);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.colors.data());
    readImage("../res/buddha/negz.jpg", tex);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.colors.data());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    //glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // ============================
    // Create FBO

    // alloc screen-size color texture
    glGenTextures(1, &gbufferColor);
    glBindTexture(GL_TEXTURE_RECTANGLE, gbufferColor);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_RECTANGLE, 0);
    // alloc screen-size depth texture
    glGenTextures(1, &gbufferDepth);
    glBindTexture(GL_TEXTURE_RECTANGLE, gbufferDepth);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT32, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_RECTANGLE, 0);

    // create and setup FBO
    glGenFramebuffers(1, &frameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, gbufferColor, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, gbufferDepth, 0);
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "incomplete framebuffer: " << fboStatus << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    glDeleteTextures(3, texName);

    glUseProgram(0);
    glDeleteProgram(shaderProgram);
}

void draw() {
    // bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.00, 0.33, 0.62, 1.0);
    glEnable(GL_DEPTH_TEST); // depth is not cleared with disabled depth test
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 0.05f, 20.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);
    glm::mat4 viewProj = projMatrix * viewMatrix;
    glm::mat4 inverseViewProj = glm::inverse(viewProj);

    { // background

        // setup shader
        glUseProgram(shaderProgramBG);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramBG, "uInverseViewProjection"), 1, GL_FALSE, glm::value_ptr(inverseViewProj));

        // bind cubemap to unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texCubeMap);
        glUniform1i(glGetUniformLocation(shaderProgramBG, "uTexCube"), 0);

        // bind any VAO and draw 6 vertices without zBuffer usage
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(vertexArrayObjectTriangle);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // use shader
    glUseProgram(shaderProgram);
    glEnable(GL_DEPTH_TEST);

    // bind texture to unit 0-2
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexColor"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texName[1]);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexNormal"), 1);

    // set light information
    glm::vec3 lightPos = glm::vec3(glm::cos(runtime) * 8, 5, glm::sin(runtime) * 8);
    glUniform3fv(glGetUniformLocation(shaderProgram, "uLightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "uCamPos"), 1, glm::value_ptr(cameraPos));
    // set time uniform
    glUniform1f(glGetUniformLocation(shaderProgram, "uRuntime"), runtime);
    // set mode uniform
    glUniform1i(glGetUniformLocation(shaderProgram, "uMode"), mode);
    // set camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewProjection"), 1, GL_FALSE, glm::value_ptr(viewProj));

    // bind VAO
    glBindVertexArray(vertexArrayObjectTriangle);
    // draw the first 3x12 indices as a triangle list
    glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_SHORT, 0);

    { // post-process and real screen draw
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(shaderProgramPost);

        // bind color to unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_RECTANGLE, gbufferColor);
        glUniform1i(glGetUniformLocation(shaderProgramPost, "uTexColor"), 0);
        // bind depth to unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_RECTANGLE, gbufferDepth);
        glUniform1i(glGetUniformLocation(shaderProgramPost, "uTexDepth"), 1);

        // bind any VAO and draw 6 vertices without zBuffer usage
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(vertexArrayObjectTriangle);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
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
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        mode = 5;
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        mode = 6;
}

// Add main() and helper implementations
#include <common-main.cc>
