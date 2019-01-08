#include <common-main.hh>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};
struct PartBaseVertex {
    glm::vec2 coord;
};
struct PartInstanceVertex {
    glm::vec3 position;
    float progress;
    float random;
};
struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float lifetime = 0;
    float maxlife = 0;
    float random;
};

GLuint arrayBufferTriangle;
GLuint elementArrayBufferTriangle;
GLuint vertexArrayObjectTriangle;


GLuint arrayBufferParticleBase;
GLuint arrayBufferParticles;
GLuint vertexArrayObjectParticles;

std::vector<Particle> particles;

GLuint shaderProgram;
GLuint shaderProgramParticle;

glm::vec3 cameraPos = glm::vec3(1.5, 1.5, 1.5);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);

void initResources() {
    std::cout << "init resources " << windowWidth << " x " << windowHeight << std::endl;

    // ============================
    // create shader (ground)
    {
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
    }
    // create shader (particles)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("particles.vs.glsl");
        std::string codeFS = readFile("particles.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramParticle = glCreateProgram();
        glAttachShader(shaderProgramParticle, vertexShader);
        glAttachShader(shaderProgramParticle, fragmentShader);
        glLinkProgram(shaderProgramParticle);

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
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // "unbind"

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3 * 2, indices, GL_STATIC_DRAW);
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

    // bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);

    glBindVertexArray(0); // "unbind" VAO

    // ============================
    // Particle setup
    glGenBuffers(1, &arrayBufferParticleBase);
    glGenBuffers(1, &arrayBufferParticles);
    glGenVertexArrays(1, &vertexArrayObjectParticles);
    PartBaseVertex pvertices[6] = {
            {{0, 0}},
            {{1, 0}},
            {{1, 1}},

            {{0, 0}},
            {{1, 1}},
            {{0, 1}}
    };
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferParticleBase);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PartBaseVertex) * 6, pvertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // setup VAO
    glBindVertexArray(vertexArrayObjectParticles);

    // particle base data
    {
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferParticleBase);

        // loc 0: quad coordinate
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PartBaseVertex), (void *) offsetof(PartBaseVertex, coord));
    }

    // particle instance data
    {
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferParticles);

        // loc 1: particle position, DIVISOR 1
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PartInstanceVertex), (void *) offsetof(PartInstanceVertex, position));
        glVertexAttribDivisor(1, 1); // set Divisor to 1

        // loc 2: particle progress, DIVISOR 1
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(PartInstanceVertex), (void *) offsetof(PartInstanceVertex, progress));
        glVertexAttribDivisor(2, 1); // set Divisor to 1

        // loc 3: particle random, DIVISOR 1
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(PartInstanceVertex), (void *) offsetof(PartInstanceVertex, random));
        glVertexAttribDivisor(3, 1); // set Divisor to 1
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

float randf(float min, float max) {
    return min + rand() / float(RAND_MAX) * (max - min);
}

void draw() {

    // particle simulation
    {
        // simulate
        static float accum = 0.0;
        static float lastTime = runtime;
        const float timeStep = 1 / 100.0f;
        accum += runtime - lastTime;
        lastTime = runtime;
        while (particles.size() < 100)
            particles.push_back({});
        while (accum > timeStep) {
            accum -= timeStep;

            for (Particle &p : particles) {
                p.lifetime += timeStep;

                // death and birth
                if (p.lifetime >= p.maxlife) {
                    p.lifetime = 0;
                    p.maxlife = randf(2, 5);
                    p.random = randf(0, 1);
                    p.position = glm::vec3(0, -1, 0);
                    p.velocity = glm::vec3(
                            randf(-.5f, .5f),
                            randf(2, 3),
                            randf(-.5f, .5f)
                    );
                }

                // sim
                p.velocity -= glm::vec3(0, 1, 0) * timeStep;
                p.position += p.velocity * timeStep;
            }
        }

        // prepare vertex data
        std::vector<PartInstanceVertex> parts;
        parts.resize(particles.size());
        for (size_t i = 0; i < particles.size(); ++i) {
            auto const &p = particles[i];
            auto &pi = parts[i];
            pi.position = p.position;
            pi.progress = p.lifetime / p.maxlife;
            pi.random = p.random;
        }

        // upload (with GL_STREAM_DRAW)
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferParticles);
        glBufferData(GL_ARRAY_BUFFER, sizeof(parts[0]) * parts.size(), parts.data(), GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.00, 0.33, 0.62, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 modelMatrix = glm::mat4();
    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 0.05f, 30.0f);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);
    glm::mat4 modelViewProj = projMatrix * viewMatrix * modelMatrix;
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

    // Draw ground
    {
        glUseProgram(shaderProgram);

        // set time uniform
        glUniform1f(glGetUniformLocation(shaderProgram, "uRuntime"), runtime);
        // set camera matrix
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModelViewProjection"), 1, GL_FALSE, glm::value_ptr(modelViewProj));

        // bind VAO
        glBindVertexArray(vertexArrayObjectTriangle);
        // draw the first 2 indices as a triangle list
        glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_SHORT, 0);
    }

    // Draw particles
    {
        glUseProgram(shaderProgramParticle);

        // set time uniform
        glUniform1f(glGetUniformLocation(shaderProgramParticle, "uRuntime"), runtime);
        // set camera matrix
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramParticle, "uModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramParticle, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));

        // bind VAO
        glBindVertexArray(vertexArrayObjectParticles);
        // draw particles as instances
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (int)particles.size());
    }
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // nothing special
}

// Add main() and helper implementations
#include <common-main.cc>
