#include <common-main.hh>
#include <suzanne.cc>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

GLuint arrayBufferPlane;
GLuint elementArrayBufferPlane;
GLuint vertexArrayObjectPlane;

GLuint arrayBufferObj;
GLuint arrayBufferObj2;
GLuint elementArrayBufferObj;
GLuint vertexArrayObjectObj;

GLuint shaderProgramPlane;
GLuint shaderProgramObj;
GLuint shaderProgramShadow;

const int shadowSize = 512;
bool shadowOffset = false;
bool shadowSelf = false;
GLuint shadowFBO;
GLuint shadowTexture;

glm::vec3 cameraPos = glm::vec3(1.5, 1.5, 1.5);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);
glm::vec3 lightPos = glm::vec3(-1, 10, 5);

void initResources() {
    std::cout << "init resources " << windowWidth << " x " << windowHeight << std::endl;

    // ============================
    // create shader (ground)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("object.vs.glsl");
        std::string codeFS = readFile("plane.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramPlane = glCreateProgram();
        glAttachShader(shaderProgramPlane, vertexShader);
        glAttachShader(shaderProgramPlane, fragmentShader);
        glLinkProgram(shaderProgramPlane);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    // create shader (object)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("object.vs.glsl");
        std::string codeFS = readFile("object.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramObj = glCreateProgram();
        glAttachShader(shaderProgramObj, vertexShader);
        glAttachShader(shaderProgramObj, fragmentShader);
        glLinkProgram(shaderProgramObj);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    // create shader (shadow cast)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("object.vs.glsl");
        std::string codeFS = readFile("shadow.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramShadow = glCreateProgram();
        glAttachShader(shaderProgramShadow, vertexShader);
        glAttachShader(shaderProgramShadow, fragmentShader);
        glLinkProgram(shaderProgramShadow);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // ============================
    // generate a GPU buffer for our triangle
    {
        glGenBuffers(1, &arrayBufferPlane);
        // .. and an index buffer
        glGenBuffers(1, &elementArrayBufferPlane);

        // upload vertex data
        Vertex vertices[4] = {
                {glm::vec3(-100, -1, -100), glm::vec3(0, 1, 0)},
                {glm::vec3(-100, -1, +100), glm::vec3(0, 1, 0)},
                {glm::vec3(+100, -1, -100), glm::vec3(0, 1, 0)},
                {glm::vec3(+100, -1, +100), glm::vec3(0, 1, 0)},
        };
        uint16_t indices[3 * 2] = {
                // bottom
                0, 2, 1,
                1, 2, 3,
        };
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferPlane);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // "unbind"

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferPlane);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3 * 2, indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // generate VAO descriptor
        glGenVertexArrays(1, &vertexArrayObjectPlane);
        // configure vertex array object (VAO)
        glBindVertexArray(vertexArrayObjectPlane);

        // location 0: position as vec3
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferPlane);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

        // location 1: normal as vec3
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferPlane);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

        // bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferPlane);

        glBindVertexArray(0); // "unbind" VAO
    }

    // ============================
    // Object setup
    {
        glGenBuffers(1, &arrayBufferObj);
        glGenBuffers(1, &arrayBufferObj2);
        // .. and an index buffer
        glGenBuffers(1, &elementArrayBufferObj);

        // upload vertex data
        suzanne_calc_normals();

        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj);
        glBufferData(GL_ARRAY_BUFFER, sizeof(suzanne_vertices), suzanne_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(suzanne_vertices), suzanne_normals, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferObj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(suzanne_indices), suzanne_indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // generate VAO descriptor
        glGenVertexArrays(1, &vertexArrayObjectObj);
        // configure vertex array object (VAO)
        glBindVertexArray(vertexArrayObjectObj);

        // location 0: position as vec3
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);

        // location 1: normal as vec3
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj2);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);

        // bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferObj);

        glBindVertexArray(0); // "unbind" VAO
    }

    // ============================
    // Shadow FBO Setup
    {
        glGenTextures(1, &shadowTexture);
        // use GL_TEXTURE_RECTANGLE and not GL_TEXTURE_2D because of mipmaps!
        glBindTexture(GL_TEXTURE_RECTANGLE, shadowTexture);
        // DEPTH_COMP is used twice!
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT32, shadowSize, shadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_RECTANGLE, 0);

        glGenFramebuffers(1, &shadowFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, shadowTexture, 0);
        glDrawBuffer(GL_NONE);
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer incomplete: " << status << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
    }

    // ============================
    // Enable zBuffering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void deleteResources() {
    // proper cleanup of resources
    // TODO
}

void draw() {

    glm::mat4 modelMatrix = glm::rotate(runtime, glm::vec3(0, 1, 0));
    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 0.05f, 30.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);

    glm::mat4 shadowProjMatrix = glm::perspective(glm::pi<float>() / 5.0f, 1.0f, 1.0f, 30.0f);
    glm::mat4 shadowViewMatrix = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0, 1, 0));
    glm::mat4 shadowViewProjMatrix = shadowProjMatrix * shadowViewMatrix;

    // Draw shadow geometry
    {
        glDrawBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

        // set viewport to shadowmap size
        glViewport(0, 0, shadowSize, shadowSize);
        // clear depth only
        glClear(GL_DEPTH_BUFFER_BIT);

        // Draw object
        {
            glUseProgram(shaderProgramShadow);

            // set camera matrix
            glUniformMatrix4fv(glGetUniformLocation(shaderProgramShadow, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgramShadow, "uViewMatrix"), 1, GL_FALSE, glm::value_ptr(shadowViewMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgramShadow, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(shadowProjMatrix));
            glUniform3fv(glGetUniformLocation(shaderProgramShadow, "uLightPos"), 1, glm::value_ptr(lightPos));

            // bind and draw VAO
            glBindVertexArray(vertexArrayObjectObj);
            glDrawElements(GL_TRIANGLES, sizeof(suzanne_indices) / sizeof(suzanne_indices[0]), GL_UNSIGNED_SHORT, 0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.00, 0.33, 0.62, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw ground
    {
        glUseProgram(shaderProgramPlane);

        // set camera matrix
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramPlane, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramPlane, "uViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramPlane, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniform3fv(glGetUniformLocation(shaderProgramPlane, "uLightPos"), 1, glm::value_ptr(lightPos));

        // bind shadow tex to unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_RECTANGLE, shadowTexture);
        glUniform1i(glGetUniformLocation(shaderProgramPlane, "uTexShadow"), 0);
        glUniform1f(glGetUniformLocation(shaderProgramPlane, "uTexShadowSize"), (float)shadowSize);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramPlane, "uShadowViewProjMatrix"), 1, GL_FALSE, glm::value_ptr(shadowViewProjMatrix));

        // bind and draw VAO
        glBindVertexArray(vertexArrayObjectPlane);
        glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_SHORT, 0);
    }
    // Draw object
    {
        glUseProgram(shaderProgramObj);

        // set camera matrix
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramObj, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramObj, "uViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramObj, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniform3fv(glGetUniformLocation(shaderProgramObj, "uLightPos"), 1, glm::value_ptr(lightPos));

        // bind shadow tex to unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_RECTANGLE, shadowTexture);
        glUniform1i(glGetUniformLocation(shaderProgramObj, "uTexShadow"), 0);;
        glUniform1f(glGetUniformLocation(shaderProgramObj, "uTexShadowSize"), (float)shadowSize);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramObj, "uShadowViewProjMatrix"), 1, GL_FALSE, glm::value_ptr(shadowViewProjMatrix));
        glUniform1f(glGetUniformLocation(shaderProgramObj, "uShadowOffset"), shadowSelf ? shadowOffset ? 0.001f : 0.0f : 1.0f);

        // bind and draw VAO
        glBindVertexArray(vertexArrayObjectObj);
        glDrawElements(GL_TRIANGLES, sizeof(suzanne_indices) / sizeof(suzanne_indices[0]), GL_UNSIGNED_SHORT, 0);
    }

}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // nothing special

    if (action == GLFW_PRESS) {

        switch (key) {
            case GLFW_KEY_1:
                lightPos = glm::vec3(-1, 10, 5);
                break;
            case GLFW_KEY_2:
                lightPos = glm::vec3(-1, 6, 5);
                break;
            case GLFW_KEY_3:
                lightPos = glm::vec3(-1, 2, 5);
                break;

            case GLFW_KEY_O:
                shadowOffset = !shadowOffset;
                break;
            case GLFW_KEY_S:
                shadowSelf = !shadowSelf;
                break;

            default:
                break;
        }

    }
}

// Add main() and helper implementations
#include <common-main.cc>
