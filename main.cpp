#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGL_renderer/src/window.hpp"
#include "OpenGL_renderer/src/models/sprite.hpp"

using namespace std;

void APIENTRY openglCallbackFunction(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar *message,
                                     const void *userParam) {

    cout << "---------------------opengl-callback-start------------" << endl;
    cout << "message: " << message << endl;
    cout << "type: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            cout << "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            cout << "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            cout << "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            cout << "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            cout << "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            cout << "OTHER";
            break;
    }
    cout << endl;

    cout << "id: " << id << endl;
    cout << "severity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_LOW:
            cout << "LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            cout << "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            cout << "HIGH";
            break;
    }
    cout << endl;
    cout << "---------------------opengl-callback-end--------------" << endl;
}

float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main() {
    window::init();
    window win(600, 600);
    win.createWindow();
    glfwSetFramebufferSizeCallback(win.getHNDL(), [](GLFWwindow *win, int w, int h) {
        glViewport(0, 0, w, h);
    });
    glfwSwapInterval(0);
    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openglCallbackFunction, nullptr);

    sprite player(win);
    player.stepX(3);
    player.stepX(-1);
    player.stepY(2);

    sprite field[10][10];

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j) {
            field[i][j] = sprite(win);
            field[i][j].stepX(i);
            field[i][j].stepY(j);
            field[i][j].setColor(color{static_cast<uint8_t>(i * 20), 0, static_cast<uint8_t>(j * 20), 0});
        }


    while (!win.run()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwSwapBuffers(win.getHNDL());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //player.update();
        //player.render();

        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j) {
                field[i][j].update();
                field[i][j].render();
            }

        std::cout << 1 / deltaTime << std::endl;

        glfwPollEvents();
    }

    return 0;
}