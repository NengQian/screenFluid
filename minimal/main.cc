#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    // internal GLFW initialization
    glfwInit();
    // create a 800x600 px window
    auto window = glfwCreateWindow(800, 600, "Basic Techniques OpenGL Lecture", NULL, NULL);
    // activate OpenGL context
    glfwMakeContextCurrent(window);
    // load OpenGL functions from driver
    gladLoadGL();

    // main loop: while window open
    while (!glfwWindowShouldClose(window)) {
        // let glfw react to external events
        glfwPollEvents();

        // set clear color state
        glClearColor(0.00, 0.33, 0.62, 1.0);
        // execute actual clear (color only)
        glClear(GL_COLOR_BUFFER_BIT);

        // swap front- and backbuffer to show current frame
        glfwSwapBuffers(window);
    }

    // cleanup window and glfw
    glfwDestroyWindow(window);
    glfwTerminate();
}