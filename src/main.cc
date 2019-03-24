#include <common-main.hh>
#include "sim_record.hpp"
#include "Particle.hpp"
#include "sim_loader.hpp"

#include <iostream>
#include <fstream>
#include <string>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct PartBaseVertex {
    glm::vec2 coord;
};

struct PartInstanceVertex {
    glm::vec3 position;
    float density;
};


struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float density;
};

GLuint arrayBufferTriangle;
GLuint elementArrayBufferTriangle;
GLuint vertexArrayObjectTriangle;

GLuint arrayBufferParticleBase;
GLuint arrayBufferParticles;
GLuint vertexArrayObjectParticles;



GLuint framebuffers[3];
GLuint color_texts[3];
GLuint depth_texts[3];
//std::vector<Particle> particles;
std::vector<std::vector<Particle>> particles_series;

int total_p;
int count = 0;

GLuint texName;

GLuint shaderProgram;
GLuint shaderProgramParticle;
GLuint shaderProgramGetDepth;
GLuint shaderProgramSmooth;
GLuint shaderProgramSurface;
GLuint shaderAnderas;


glm::vec3 cameraPos = glm::vec3(0, 0, 0.5);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);


// load and set up particles_series

std::vector<Particle> convert_particles(std::vector<mParticle>& mps)
{
	std::vector<Particle> ps;

	for (auto& mp : mps)
	{

		Particle p;
		p.position = glm::vec3(mp.position[0], mp.position[2], mp.position[1]);
		p.velocity = glm::vec3(mp.velocity[0], mp.velocity[2], mp.velocity[1]);
		p.density = static_cast<float>(mp.density);

		ps.push_back(p);
	}

	return ps;
}

void load_particle_series(std::string fs) {
	using namespace std::string_literals;

	SimData simData(fs);

	particles_series.clear();

	for (int i=0; i<simData.total_frame_num; ++i)
	{
		std::vector<Particle> ps = convert_particles(simData.sim_rec.states[i].particles);

		particles_series.push_back(ps);
	}
}

void initResources() {
    std::cout << "init resources " << windowWidth << " x " << windowHeight << std::endl;

    // ===========================
    // load simulation data
    load_particle_series("../../testFile/dan_WCSPH_n10_t0001_10000_density.bin");
    //load_particle_series("./my_hot_pot2.bin");

    total_p = particles_series[0].size();

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
    // {
    //     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //     // setup shader source
    //     std::string codeVS = readFile("particles.vs.glsl");
    //     std::string codeFS = readFile("particles.fs.glsl");
    //     const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
    //     glShaderSource(vertexShader, 1, &cVS, nullptr);
    //     glShaderSource(fragmentShader, 1, &cFS, nullptr);
    //     // compile shader
    //     glCompileShader(vertexShader);
    //     glCompileShader(fragmentShader);
    //     checkShader(vertexShader);
    //     checkShader(fragmentShader);

    //     // create shader program
    //     shaderProgramParticle = glCreateProgram();
    //     glAttachShader(shaderProgramParticle, vertexShader);
    //     glAttachShader(shaderProgramParticle, fragmentShader);
    //     glLinkProgram(shaderProgramParticle);

    //     // delete unused shader
    //     glDeleteShader(vertexShader);
    //     glDeleteShader(fragmentShader);
    // }



        // create shader (depth texture before smoothing)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("particles.vs.glsl");
        std::string codeFS = readFile("get_depth.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
        shaderProgramGetDepth = glCreateProgram();
        glAttachShader(shaderProgramGetDepth, vertexShader);
        glAttachShader(shaderProgramGetDepth, fragmentShader);
        glLinkProgram(shaderProgramGetDepth);

        GLint isLinked = 0;
        glGetProgramiv( shaderProgramGetDepth, GL_LINK_STATUS, &isLinked);
        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }


    // create shader (smooth shader)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("postprocessing.vs.glsl");
        std::string codeFS = readFile("smooth.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
         shaderProgramSmooth = glCreateProgram();
        glAttachShader( shaderProgramSmooth, vertexShader);
        glAttachShader( shaderProgramSmooth, fragmentShader);
        glLinkProgram( shaderProgramSmooth);

        GLint isLinked = 0;
        glGetProgramiv( shaderProgramSmooth, GL_LINK_STATUS, &isLinked);


        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }


        // create shader (smooth shader)
    // {
    //     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //     // setup shader source
    //     std::string codeVS = readFile("postprocessing.vs.glsl");
    //     std::string codeFS = readFile("smooth.fs.glsl");
    //     const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
    //     glShaderSource(vertexShader, 1, &cVS, nullptr);
    //     glShaderSource(fragmentShader, 1, &cFS, nullptr);
    //     // compile shader
    //     glCompileShader(vertexShader);
    //     glCompileShader(fragmentShader);
    //     checkShader(vertexShader);
    //     checkShader(fragmentShader);

    //     // create shader program
    //      shaderProgramSmooth = glCreateProgram();
    //     glAttachShader( shaderProgramSmooth, vertexShader);
    //     glAttachShader( shaderProgramSmooth, fragmentShader);
    //     glLinkProgram( shaderProgramSmooth);

    //     GLint isLinked = 0;
    //     glGetProgramiv( shaderProgramSmooth, GL_LINK_STATUS, &isLinked);


    //     // delete unused shader
    //     glDeleteShader(vertexShader);
    //     glDeleteShader(fragmentShader);
    // }


        // create surface shader
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // setup shader source
        std::string codeVS = readFile("postprocessing.vs.glsl");
        std::string codeFS = readFile("surface.fs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);

        // create shader program
         shaderProgramSurface = glCreateProgram();
        glAttachShader( shaderProgramSurface, vertexShader);
        glAttachShader( shaderProgramSurface, fragmentShader);
        glLinkProgram( shaderProgramSurface);

        GLint isLinked = 0;
        glGetProgramiv( shaderProgramSurface, GL_LINK_STATUS, &isLinked);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }


    // create andreas shader
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        // setup shader source
        std::string codeVS = readFile("andreas.vs.glsl");
        std::string codeFS = readFile("andreas.fs.glsl");
        std::string codeGS = readFile("andreas.gs.glsl");
        const char *cVS = codeVS.c_str(), *cFS = codeFS.c_str(), *cGS = codeGS.c_str();
        glShaderSource(vertexShader, 1, &cVS, nullptr);
        glShaderSource(fragmentShader, 1, &cFS, nullptr);
        glShaderSource(geometryShader, 1, &cGS, nullptr);
        // compile shader
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        glCompileShader(geometryShader);
        checkShader(vertexShader);
        checkShader(fragmentShader);
        checkShader(geometryShader);

        // create shader program
         shaderAnderas = glCreateProgram();
        glAttachShader( shaderAnderas, vertexShader);
        glAttachShader( shaderAnderas, fragmentShader);
        glAttachShader( shaderAnderas, geometryShader);
        glLinkProgram( shaderAnderas);

        GLint isLinked = 0;
        glGetProgramiv( shaderAnderas, GL_LINK_STATUS, &isLinked);

        // delete unused shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);
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
    glGenVertexArrays(1, &vertexArrayObjectTriangle);  //what is vertexarray?? why when we configure the vertexarray, we actually only operate the arraybuffer?
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferTriangle);  //why bind several time??? why need unbind ?

    glBindVertexArray(0); // "unbind" VAO     //neng, can we actually see this bind and unbind vao as a pair of braket? like all code inside this bracket pair are setting this VAO, or setting how these data in buffer can be read.

    

    for(int i =0;i<3;++i)
    {
        glGenFramebuffers(1, &framebuffers[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);

        glGenTextures(1, &color_texts[i]);
        glBindTexture(GL_TEXTURE_2D, color_texts[i]);
        glTexStorage2D(GL_TEXTURE_2D, 9, GL_RGBA32F, 512, 512);
        // close the mipmap?
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // glGenTextures(1, &depth_texts[i]);
        // glBindTexture(GL_TEXTURE_2D, depth_texts[i]);
        // glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 512, 512);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texts[i], 0);
        // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texts[i], 0);

        // check if driver objects
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cout<<"incomplete framebuffer"<<std::endl;

        static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, draw_buffers);   // neng: should I run here?
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // // create fbo 2, which store y direction
    // // create FBO
    // glGenFramebuffers(1, &framebufferY);
    // glBindFramebuffer(GL_FRAMEBUFFER, framebufferY);
    // glGenTextures(1, &color_text_y);
    // glBindTexture(GL_TEXTURE_2D, color_text_y);
    // glTexStorage2D(GL_TEXTURE_2D, 9, GL_RGBA8, 512, 512);
    // // close the mipmap?
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glGenTextures(1, &depth_text_y);
    // glBindTexture(GL_TEXTURE_2D, depth_text_y);
    // glTexStorage2D(GL_TEXTURE_2D, 9, GL_DEPTH_COMPONENT32F, 512, 512);

    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_text_y, 0);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_text_y, 0);

    // // check if driver objects
    // auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    // if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    //     std::cout<<"incomplete framebuffer"<<std::endl;

    // static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
    // glDrawBuffers(1, draw_buffers);   // neng: should I run here?
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // ============================
    // Particle setup
    glGenBuffers(1, &arrayBufferParticles);
    glGenVertexArrays(1, &vertexArrayObjectParticles);

    // setup VAO
    glBindVertexArray(vertexArrayObjectParticles);

    // particle instance data
    {
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferParticles);

        // loc 0: particle position, DIVISOR 0
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PartInstanceVertex), (void *) offsetof(PartInstanceVertex, position));
        // loc 1: density
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1 ,1, GL_FLOAT, GL_FALSE,sizeof(PartInstanceVertex),(void *) offsetof(PartInstanceVertex, density));

    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // "unbind" VAO

    // Enable zBuffering
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void deleteResources() {
    // proper cleanup of resources
    glDeleteVertexArrays(1, &vertexArrayObjectTriangle);
    glDeleteBuffers(1, &arrayBufferTriangle);
    glDeleteBuffers(1, &elementArrayBufferTriangle);

    glDeleteVertexArrays(1, &vertexArrayObjectParticles);
    glDeleteBuffers(1, &arrayBufferParticles);

    for (int i=0; i<3; ++i)
    {
        glDeleteFramebuffers(1, &framebuffers[i]);
        glDeleteTextures(1, &color_texts[i]);
        // glDeleteTextures(1, &depth_texts[i]);
    }

    glUseProgram(0);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgramGetDepth);
    glDeleteProgram(shaderProgramSmooth);
    glDeleteProgram(shaderProgramSurface);
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

        // prepare vertex data
        static std::vector<PartInstanceVertex> parts;

        if (parts.empty())
        	parts.resize(particles_series[count].size());
   
        for (size_t i = 0; i < particles_series[count].size(); ++i) {
            auto const &p = particles_series[count][i];
            auto &pi = parts[i];
            pi.position = p.position;
            pi.density = p.density;
        }


        if (count < particles_series.size()-2)
        	++count;
            
        // upload (with GL_STREAM_DRAW)
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferParticles);
        glBufferData(GL_ARRAY_BUFFER, sizeof(parts[0]) * parts.size(), parts.data(), GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.0, 1.0, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 modelMatrix = glm::mat4();
    glm::mat4 projMatrix = glm::perspective(glm::pi<float>() / 2.0f, windowWidth / (float) windowHeight, 1.0f, 7.0f);
    glm::mat4 invProjMatrix = glm::inverse(projMatrix);
    glm::mat4 viewMatrix = interactiveView(&cameraPos, &cameraTarget);
    glm::mat4 modelViewProj = projMatrix * viewMatrix * modelMatrix;
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
    glm::vec2 screenSize = glm::vec2(windowWidth, windowHeight);
    float maxDepth = 7.0;
    float normalDepth = 10.0;
    float spriteSize = 0.10;
    glDepthRange(0,1);

 

    //get depth
    {
        // glEnable(GL_PROGRAM_POINT_SIZE);
        // glDepthMask(GL_TRUE);
        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);
        // // glViewport(0, 0, windowWidth, windowHeight);
        // // glClearColor(0.00, 0.33, 0.62, 1.0);
        // // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // glViewport(0, 0, windowWidth, windowHeight);
        // glClearColor(0.0, 1.0, 0.8, 1.0);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glUseProgram(shaderProgramGetDepth);

        // glUniformMatrix4fv(glGetUniformLocation(shaderProgramGetDepth, "uModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        // glUniformMatrix4fv(glGetUniformLocation(shaderProgramGetDepth, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        // glUniform2fv(glGetUniformLocation(shaderProgramGetDepth, "uScreenSize"), 1, glm::value_ptr(screenSize));
        // glUniform1f(glGetUniformLocation(shaderProgramGetDepth, "uSpriteSize"), spriteSize);
        // glUniform1f(glGetUniformLocation( shaderProgramGetDepth, "uNormdepth"), normalDepth);


        // glBindVertexArray(vertexArrayObjectParticles);
        // //std::cout<<"totoal_p" << total_p <<std::endl;
        // glDrawArraysInstanced(GL_POINTS, 0, total_p, total_p);
        // //glDrawArrays(GL_POINTS, 0, total_p);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // // glDepthMask(GL_TRUE);
        // // glDisable(GL_PROGRAM_POINT_SIZE);



    }


        // Anderas shader
    {
        //glEnable(GL_PROGRAM_POINT_SIZE);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        // glViewport(0, 0, windowWidth, windowHeight);
        // glClearColor(0.00, 0.33, 0.62, 1.0);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);

        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(1.0, 1.0, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderAnderas);


        glUniformMatrix4fv(glGetUniformLocation(shaderAnderas, "view"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderAnderas, "projection"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderAnderas, "inv_projection"), 1, GL_FALSE, glm::value_ptr(invProjMatrix));

        glUniform1f(glGetUniformLocation(shaderAnderas, "viewport_width"), screenSize.x);
        glUniform1f(glGetUniformLocation(shaderAnderas, "viewport_height"), screenSize.y);
        glUniform1f(glGetUniformLocation(shaderAnderas, "near_plane_dist"), 1.0);

        glUniform1f(glGetUniformLocation(shaderAnderas, "radius"), spriteSize);
        glUniform1f(glGetUniformLocation( shaderAnderas, "uNormdepth"), normalDepth);



        glBindVertexArray(vertexArrayObjectParticles);
        glDrawArrays(GL_POINTS, 0, total_p);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // glDepthMask(GL_TRUE);
        // glDisable(GL_PROGRAM_POINT_SIZE);
    }



    for(int i =0;i<10;i++)
{
   //smooth in x direction
    {
        //glEnable(GL_PROGRAM_POINT_SIZE);
        glDisable(GL_PROGRAM_POINT_SIZE);
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffers[1]);


        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindTexture(GL_TEXTURE_2D, color_texts[0]);
        //glBindTexture(GL_TEXTURE_2D, depth_text);

        glUseProgram( shaderProgramSmooth);

        glUniformMatrix4fv(glGetUniformLocation( shaderProgramSmooth, "uModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation( shaderProgramSmooth, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniform2fv(glGetUniformLocation( shaderProgramSmooth, "uScreenSize"), 1, glm::value_ptr(screenSize));
        glUniform1f(glGetUniformLocation( shaderProgramSmooth, "uSpriteSize"), spriteSize);
        glm::vec2 dir_x = glm::vec2(1.0, 0.0);
        glUniform2fv(glGetUniformLocation( shaderProgramSmooth, "dir"),1 ,glm::value_ptr(dir_x));
        glUniform1f(glGetUniformLocation( shaderProgramSmooth, "uNormdepth"), normalDepth);
        glUniform1f(glGetUniformLocation( shaderProgramSmooth, "uMaxdepth"), maxDepth);


        glBindVertexArray(vertexArrayObjectParticles);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);

        //glDrawArraysInstanced(GL_POINTS, 0, total_p, total_p);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //glDepthMask(GL_TRUE);
        //glDisable(GL_PROGRAM_POINT_SIZE);
    }

        //smooth in y direction
    {
        //glEnable(GL_PROGRAM_POINT_SIZE);
      //  glDisable(GL_PROGRAM_POINT_SIZE);

        glBindFramebuffer(GL_FRAMEBUFFER,framebuffers[0]);


        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindTexture(GL_TEXTURE_2D, color_texts[1]);
        //glBindTexture(GL_TEXTURE_2D, depth_text);

        glUseProgram( shaderProgramSmooth);

        glUniformMatrix4fv(glGetUniformLocation( shaderProgramSmooth, "uModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation( shaderProgramSmooth, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniform2fv(glGetUniformLocation( shaderProgramSmooth, "uScreenSize"), 1, glm::value_ptr(screenSize));
        glUniform1f(glGetUniformLocation( shaderProgramSmooth, "uSpriteSize"), spriteSize);
        glm::vec2 dir_y = glm::vec2(0.0, 1.0);
        glUniform2fv(glGetUniformLocation( shaderProgramSmooth, "dir"),1 ,glm::value_ptr(dir_y));
        glUniform1f(glGetUniformLocation( shaderProgramSmooth, "uNormdepth"), normalDepth);
        glUniform1f(glGetUniformLocation( shaderProgramSmooth, "uMaxdepth"), maxDepth);

        glBindVertexArray(vertexArrayObjectParticles);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);

    //    glDrawArraysInstanced(GL_POINTS, 0, total_p, total_p);
       glBindFramebuffer(GL_FRAMEBUFFER, 0);

     //   glDepthMask(GL_TRUE);
     //   glDisable(GL_PROGRAM_POINT_SIZE);
    }
}


    //draw surface
    {

        glDisable(GL_PROGRAM_POINT_SIZE);
        //glDisable(GL_PROGRAM_POINT_SIZE);
        //glDepthMask(GL_TRUE);
        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);
 

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(1.0, 1.0, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindTexture(GL_TEXTURE_2D, color_texts[0]);
        //glBindTexture(GL_TEXTURE_2D, depth_text);

        glUseProgram( shaderProgramSurface);

        glUniformMatrix4fv(glGetUniformLocation( shaderProgramSurface, "uModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation( shaderProgramSurface, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniform2fv(glGetUniformLocation( shaderProgramSurface, "uScreenSize"), 1, glm::value_ptr(screenSize));
        glUniform1f(glGetUniformLocation( shaderProgramSurface, "uSpriteSize"), spriteSize);
        glUniform1f(glGetUniformLocation( shaderProgramSurface, "uMaxdepth"), maxDepth);
        glUniform1f(glGetUniformLocation( shaderProgramSurface, "uNormdepth"), normalDepth);
        glBindVertexArray(vertexArrayObjectParticles);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //glDrawArraysInstanced(GL_POINTS, 0, total_p, total_p);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDepthMask(GL_TRUE);
        glDisable(GL_PROGRAM_POINT_SIZE);
    }

            // Anderas shader for surface 
    // {
    //     //glEnable(GL_PROGRAM_POINT_SIZE);
    //     glDepthMask(GL_TRUE);
    //     glEnable(GL_DEPTH_TEST);
    //     glDepthFunc(GL_LESS);
    //     // glViewport(0, 0, windowWidth, windowHeight);
    //     // glClearColor(0.00, 0.33, 0.62, 1.0);
    //     // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //     glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);

    //     glViewport(0, 0, windowWidth, windowHeight);
    //     glClearColor(1.0, 1.0, 0.8, 1.0);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //     glUseProgram(shaderAnderas);


    //     glUniformMatrix4fv(glGetUniformLocation(shaderAnderas, "view"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    //     glUniformMatrix4fv(glGetUniformLocation(shaderAnderas, "projection"), 1, GL_FALSE, glm::value_ptr(projMatrix));
    //     glUniformMatrix4fv(glGetUniformLocation(shaderAnderas, "inv_projection"), 1, GL_FALSE, glm::value_ptr(invProjMatrix));

    //     glUniform1f(glGetUniformLocation(shaderAnderas, "viewport_width"), screenSize.x);
    //     glUniform1f(glGetUniformLocation(shaderAnderas, "viewport_height"), screenSize.y);
    //     glUniform1f(glGetUniformLocation(shaderAnderas, "near_plane_dist"), 1.0);

    //     glUniform1f(glGetUniformLocation(shaderAnderas, "radius"), spriteSize);
    //     glUniform1f(glGetUniformLocation( shaderAnderas, "uNormdepth"), normalDepth);



    //     glBindVertexArray(vertexArrayObjectParticles);
    //     glDrawArrays(GL_POINTS, 0, total_p);
    //     glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // glDepthMask(GL_TRUE);
        // glDisable(GL_PROGRAM_POINT_SIZE);
    // }


   // Draw ground
    // {
    //     glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //     glUseProgram(shaderProgram);

    //     // set camera matrix
    //     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModelViewProjection"), 1, GL_FALSE, glm::value_ptr(modelViewProj));

    //     // bind VAO
    //     glBindVertexArray(vertexArrayObjectTriangle);
    //     // draw the first 2 indices as a triangle list
    //     glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_SHORT, 0);
    // }
}

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    ;// nothing special
}

// Add main() and helper implementations
#include <common-main.cc>
