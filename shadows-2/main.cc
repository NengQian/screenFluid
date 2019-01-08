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
GLuint shaderProgramBlurX;
GLuint shaderProgramBlurY;

const int shadowSize = 512;
GLuint shadowFBO[2];
GLuint shadowTextureDepth;
GLuint shadowTexture[2];

int blurs = 0;

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
    { // Post-process Blur X
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("postprocess.vs.glsl");
        std::string codeFS = readFile("blur-x.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramBlurX = glCreateProgram();
        glAttachShader(shaderProgramBlurX, vertexShader);
        glAttachShader(shaderProgramBlurX, fragmentShader);
        glLinkProgram(shaderProgramBlurX);
        checkProgram(shaderProgramBlurX);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    { // Post-process Blur Y
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("postprocess.vs.glsl");
        std::string codeFS = readFile("blur-y.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramBlurY = glCreateProgram();
        glAttachShader(shaderProgramBlurY, vertexShader);
        glAttachShader(shaderProgramBlurY, fragmentShader);
        glLinkProgram(shaderProgramBlurY);
        checkProgram(shaderProgramBlurY);

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
    glGenTextures(1, &shadowTextureDepth);
    // use GL_TEXTURE_RECTANGLE and not GL_TEXTURE_2D because of mipmaps!
    glBindTexture(GL_TEXTURE_RECTANGLE, shadowTextureDepth);
    // DEPTH_COMP is used twice!
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT32, shadowSize, shadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_RECTANGLE, 0);

    for (int i = 0; i < 2; ++i) {
        glGenTextures(1, &shadowTexture[i]);


        glBindTexture(GL_TEXTURE_RECTANGLE, shadowTexture[i]);
        // DEPTH_COMP is used twice!
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_R32F, shadowSize, shadowSize, 0, GL_RED, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_RECTANGLE, 0);

        glGenFramebuffers(1, &shadowFBO[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, shadowTexture[i], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, shadowTextureDepth, 0);
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer incomplete: " << status << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[0]);

        // set viewport to shadowmap size
        glViewport(0, 0, shadowSize, shadowSize);
        // clear depth and color
        glClear(GL_DEPTH_BUFFER_BIT);
        // special clear for color because values are high
        const float clearValue = glm::exp(80.0f);
        glClearBufferfv(GL_COLOR, 0, &clearValue);

        // Draw ground
        {
            glUseProgram(shaderProgramShadow);

            // set camera matrix
            glUniformMatrix4fv(glGetUniformLocation(shaderProgramShadow, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgramShadow, "uViewMatrix"), 1, GL_FALSE, glm::value_ptr(shadowViewMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgramShadow, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(shadowProjMatrix));
            glUniform3fv(glGetUniformLocation(shaderProgramShadow, "uLightPos"), 1, glm::value_ptr(lightPos));

            // bind and draw VAO
            glBindVertexArray(vertexArrayObjectPlane);
            glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_SHORT, 0);
        }
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
    }

    // Blur shadow
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(vertexArrayObjectObj); // any
    for (int i = 0; i < blurs; ++i) {
        { // post-process Blur X
            glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[1]);
            glUseProgram(shaderProgramBlurX);

            // bind color to unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_RECTANGLE, shadowTexture[0]);
            glUniform1i(glGetUniformLocation(shaderProgramBlurX, "uTexDepth"), 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        { // post-process Blur Y
            glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[0]);
            glUseProgram(shaderProgramBlurY);

            // bind color to unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_RECTANGLE, shadowTexture[1]);
            glUniform1i(glGetUniformLocation(shaderProgramBlurY, "uTexDepth"), 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        glBindTexture(GL_TEXTURE_RECTANGLE, shadowTexture[0]);
        glUniform1i(glGetUniformLocation(shaderProgramPlane, "uTexShadow"), 0);
        glUniform1f(glGetUniformLocation(shaderProgramPlane, "uTexShadowSize"), (float) shadowSize);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramPlane, "uShadowViewProjMatrix"), 1, GL_FALSE, glm::value_ptr(shadowViewProjMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramPlane, "uShadowViewMatrix"), 1, GL_FALSE, glm::value_ptr(shadowViewMatrix));

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
        glBindTexture(GL_TEXTURE_RECTANGLE, shadowTexture[0]);
        glUniform1i(glGetUniformLocation(shaderProgramObj, "uTexShadow"), 0);;
        glUniform1f(glGetUniformLocation(shaderProgramObj, "uTexShadowSize"), (float) shadowSize);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramObj, "uShadowViewProjMatrix"), 1, GL_FALSE, glm::value_ptr(shadowViewProjMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramObj, "uShadowViewMatrix"), 1, GL_FALSE, glm::value_ptr(shadowViewMatrix));

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

            case GLFW_KEY_M:
                ++blurs;
                break;
            case GLFW_KEY_N:
                blurs = glm::max(0, blurs - 1);
                break;

            default:
                break;
        }

    }
}

// Add main() and helper implementations
#include <common-main.cc>
