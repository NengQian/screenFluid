#include <common-main.hh>
#include <sstream>
#include <map>

void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods);

double internalLastX = -1, internalLastY = -1;
bool internalRotating = false;

void internalOnMouseBtn(GLFWwindow *window, int btn, int action, int mods) {
    if (btn == GLFW_MOUSE_BUTTON_LEFT)
        internalRotating = action == GLFW_PRESS;
}

void internalOnCursorPos(GLFWwindow *window, double x, double y) {
    if (internalLastX == -1) internalLastX = x;
    if (internalLastY == -1) internalLastY = y;

    float dx = float(x - internalLastX);
    float dy = float(y - internalLastY);

    internalLastX = x;
    internalLastY = y;

    if (internalRotating) {
        angleAzimuth += dx / 500.0f;
        angleAltitude = glm::clamp(angleAltitude + dy / 500.0f, glm::pi<float>() * -0.45f, glm::pi<float>() * 0.45f);
    }
}

void internalOnScroll(GLFWwindow *window, double x, double y) {
    mouseZoom *= glm::pow(1.03, -y);
    mouseZoom = glm::clamp(mouseZoom, 0.1f, 10.0f);
}

void internalOnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    onKeyPress(window, key, scancode, action, mods);

    // reset pose
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        angleAltitude = glm::pi<float>() * 0.22f;
        angleAzimuth = glm::pi<float>() * 0.20f;
        mouseZoom = 2.5f;
    }

    // close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        paused = !paused;

    // reload
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        deleteResources();
        initResources();
    }

    // wireframe
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        static bool wireframe = false;
        wireframe = !wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }
}

void openGLDebugCallback(GLenum source,
                         GLenum type,
                         GLuint id,
                         GLenum severity,
                         GLsizei length,
                         const GLchar *message,
                         const void *userParam) {
    auto stype = "unknown";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            stype = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            stype = "DEPRECATED";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            stype = "UNDEFINED";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            stype = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            stype = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_OTHER:
            stype = "OTHER";
            return; // too verbose

        default:
            break;
    }
    std::cerr << "OpenGL:" << source << ":" << stype << ": " << message << std::endl;
}

void checkError() {
    auto status = glGetError();
    if (status != GL_NO_ERROR) {
        auto msg = "Unknown error";
        switch (status) {
            case GL_INVALID_ENUM:
                msg = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                msg = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                msg = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                msg = "GL_OUT_OF_MEMORY";
                break;
            default:
                break;
        }
        std::cerr << "OpenGL Error: " << status << ": " << msg << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // setup glfw
    glfwInit();

    // create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto window = glfwCreateWindow(windowWidth, windowHeight, "Basic Techniques OpenGL Lecture", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();
    if (glDebugMessageCallbackKHR)
        glDebugMessageCallbackKHR(openGLDebugCallback, nullptr);
    std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << std::endl;

    // init resources and setup resize
    // resize is handled trivially here (delete -> init)
    initResources();
    checkError();
    glfwSetKeyCallback(window, internalOnKeyPress);
    glfwSetMouseButtonCallback(window, internalOnMouseBtn);
    glfwSetCursorPosCallback(window, internalOnCursorPos);
    glfwSetScrollCallback(window, internalOnScroll);
    glfwSetWindowSizeCallback(window, [](auto win, int w, int h) {
        deleteResources();
        windowWidth = w;
        windowHeight = h;
        initResources();
        checkError();
    });

    // main loop
    auto lastTime = glfwGetTime();
    auto fpsTime = lastTime;
    auto fpsCnt = 0;
    do {
        // react to events
        glfwPollEvents();

        // advance time
        auto newTime = glfwGetTime();
        if (!paused)
            runtime += newTime - lastTime;
        lastTime = newTime;

        // fps
        ++fpsCnt;
        if (lastTime > fpsTime + 2) {
            double fps = fpsCnt / (lastTime - fpsTime);
            std::cout << "FPS: " << fps << std::endl;
            fpsTime = lastTime;
            fpsCnt = 0;
        }

        // actual rendering
        draw();
        checkError();

        // end frame
        glfwSwapBuffers(window);

    } while (!glfwWindowShouldClose(window));

    // cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
}

std::string readFileDirect(std::string filename) {
    std::ifstream t(filename);
    if (!t.good())
        std::cerr << "Unable to open " << filename << std::endl;
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}

std::string resolvePath(std::string filename, std::string relpathOrFile) {
    if (filename.empty())
        return "";
    if (relpathOrFile.empty() || filename[0] == '/')
        return filename;

    auto slashP = relpathOrFile.find_last_of('/');
    if (slashP == std::string::npos)
        return filename;

    auto absfile = relpathOrFile.substr(0, slashP) + "/" + filename;
    return absfile;
}

void checkShader(GLuint shaderID) {
    int infoLogLength;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength <= 1)
        return;

    std::vector<char> msg((unsigned long) (infoLogLength + 1));
    glGetShaderInfoLog(shaderID, infoLogLength, NULL, msg.data());
    std::cerr << "Shader error: " << msg.data() << std::endl;
}


void checkProgram(GLuint shaderID) {
    int infoLogLength;
    glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength <= 1)
        return;

    std::vector<char> msg((unsigned long) (infoLogLength + 1));
    glGetProgramInfoLog(shaderID, infoLogLength, NULL, msg.data());
    std::cerr << "Shader Program error: " << msg.data() << std::endl;
}

void readImageDirect(std::string filename, Texture &tex) {
    static std::map<std::string, Texture> cache;
    if (cache.count(filename)) {
        tex = cache[filename];
        return;
    }

    int channels;
    uint8_t *data = SOIL_load_image(filename.c_str(), &tex.width, &tex.height, &channels, SOIL_LOAD_RGB);
    if (!data) {
        std::cerr << "Unable to open " << filename << ": " << SOIL_last_result() << std::endl;
        return;
    }

    tex.colors.resize((size_t) (tex.width * tex.height * 3));
    std::copy(data, data + tex.width * tex.height * 3, tex.colors.begin());

    cache[filename] = tex;
}

void readPPMDirect(std::string filename, Texture &tex) {
    static std::map<std::string, Texture> cache;
    if (cache.count(filename)) {
        tex = cache[filename];
        return;
    }

    std::ifstream t(filename);
    if (!t.good()) {
        std::cerr << "Unable to open " << filename << std::endl;
        return;
    }

    std::vector<std::string> lines;
    for (std::string line; std::getline(t, line);)
        if (line.size() > 0 && line[0] != '#')
            lines.push_back(line);

    if (lines[0] != "P3")
        std::cerr << "wrong tex format: " << lines[0] << ", file: " << filename << std::endl;

    std::stringstream ss;
    for (int i = 3; i < lines.size(); ++i)
        ss << " " << lines[i];
    ss.seekg(0);

    tex.colors.clear();
    std::istringstream(lines[1]) >> tex.width >> tex.height;
    for (int i = 0; i < tex.width * tex.height * 3; ++i) {
        int val;
        ss >> val;
        if (val >= 0)
            tex.colors.push_back((uint8_t) val);
    }

    if (tex.colors.size() != tex.width * tex.height * 3)
        std::cerr << "Color data does not match size! file: " << filename << std::endl;
    cache[filename] = tex;
}

std::vector<OBJVertex> readOBJDirect(std::string filename) {
    std::ifstream t(filename);
    if (!t.good()) {
        std::cerr << "Unable to open " << filename << std::endl;
        return {};
    }

    std::vector<glm::vec3> positions = {{0, 0, 0}};
    std::vector<glm::vec3> normals = {{0, 1, 0}};
    std::vector<glm::vec2> texcoords = {{0, 0}};
    std::vector<OBJVertex> verts;

    char fi[4][20];

    for (std::string line; std::getline(t, line);) {
        if (line.size() < 3)
            continue;

        if (line[0] == 'v') {
            if (line[1] == 't') {
                // tex coord
                float u, v;
                sscanf(line.c_str(), "vt %f %f", &u, &v);
                texcoords.push_back({u, v});
            } else if (line[1] == 'n') {
                // normal
                float x, y, z;
                sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
                normals.push_back({x, y, z});
            } else if (line[1] == ' ') {
                // position
                float x, y, z;
                sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
                positions.push_back({x, y, z});
            } else {
                // not supported
            }
        } else if (line[0] == 'f') {
            // face
            int count = 0;
            for (int i = 0; line[i]; i++)
                count += (line[i] == ' ');

            if (count == 3)
                sscanf(line.c_str(), "f %s %s %s", fi[0], fi[1], fi[2]);
            else if (count == 4)
                sscanf(line.c_str(), "f %s %s %s %s", fi[0], fi[1], fi[2], fi[3]);
            else
                continue;//unsupported

            for (int i = 0; i < count; ++i) {
                int iv = 0, in = 0, it = 0;
                char *f = fi[i];
                if (strstr(f, "//"))
                    sscanf(fi[i], "%d//%d", &iv, &in);
                else if (strchr(f, '/')) {
                    if (strchr(strchr(f, '/') + 1, '/'))
                        sscanf(fi[i], "%d/%d/%d", &iv, &it, &in);
                    else
                        sscanf(fi[i], "%d/%d", &iv, &it);
                } else
                    sscanf(fi[i], "%d", &iv);

                verts.push_back({positions[iv], normals[in], texcoords[it]});
            }

            if (count == 4) {
                verts.push_back(verts[verts.size() - 4]);
                verts.push_back(verts[verts.size() - 3]);
            }
        }
        else {
            // not supported
        }
    }

    return verts;
}
