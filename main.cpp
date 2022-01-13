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

static constexpr float BOUNDRY_R = 9.5;
static constexpr float BOUNDRY_L = .5;
static constexpr float SPEED_X = 4;
static constexpr float SPEED_Y = 3;

void processBoundryCollision(glm::vec2 &dir, glm::vec2 &pos) {
    if (pos[0] >= BOUNDRY_R && dir[0] > 0 ||
        pos[0] <= BOUNDRY_L && dir[0] < 0) {
        dir = glm::vec2(-dir[0], dir[1]);
    }

    if (pos[1] >= 9.5 && dir[1] > 0 ||
        pos[1] <= .5 && dir[1] < 0) {
        dir = glm::vec2(dir[0], -dir[1]);
    }
}

void processBallCollision(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 &dir1, glm::vec2 &dir2, double deltaTime) {
    auto dir1temp = dir1;
    auto dir2temp = dir2;
    dir1temp[0] *= (float)deltaTime;
    dir1temp[1] *= (float)deltaTime;
    dir2temp[0] *= (float)deltaTime;
    dir2temp[1] *= (float)deltaTime;
    pos1 += dir1temp;
    pos2 += dir2temp;
    if (glm::distance(pos1, pos2) < 1) {
        //TODO Calculate new angel
        glm::vec2 middleVec = pos2 - pos1;

        float tangentAngle = std::atan(middleVec[1] / middleVec[0]);
        tangentAngle = std::round(glm::degrees(tangentAngle));
        std::cout << std::round(glm::degrees(tangentAngle)) << std::endl;

        double posDir = pos1[0] * pos2[0] + pos1[1] * pos2[1];
        double multiplier = 1;
        if(posDir >= 180)
            multiplier = 2;
        std::cout << "posDir: " << posDir << std::endl;
        float rotation = tangentAngle * 2.f * multiplier;
        dir1 = glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0, 0, 1)) * glm::vec4(dir1, 0, 0);
        dir2 = glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0, 0, 1)) * glm::vec4(dir2, 0, 0);
        std::cout << dir1[0] << " " << dir1[1] << std::endl;



        static int count = 0;
        ++count;
        std::cout << "COLLISION NUMMBER: " << count << std::endl;
    }
}

int main() {
    window::init();
    //TODO Changing window size destroys EVERYTHING
    window win(600, 600);
    win.createWindow();
    glfwSetFramebufferSizeCallback(win.getHNDL(), [](GLFWwindow *win, int w, int h) {
        glViewport(0, 0, w, h);
    });
    glfwSwapInterval(0);
    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openglCallbackFunction, nullptr);

    /*J
    sprite ball(win);
    ball.stepX(3);
    ball.stepY(2);
    ball.setColor(color{100, 0, 100, 0});
    ball.update();
     */

    constexpr int SPRITE_COUNT = 2;

    sprite ball[SPRITE_COUNT];
    for (int i = 0; i < SPRITE_COUNT; ++i) {
        ball[i] = sprite(win);
        ball[i].stepX(static_cast<float>(i));
        ball[i].setColor(color{static_cast<uint8_t>(i * 20), 120, 200});
        ball[i].update();
    }

    float currentFrame;
    float deltaTime = 0.0f;    // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    glm::vec4 move1 = {0, 0, 0, 0};
    glm::vec4 move2 = {-SPEED_X, -SPEED_Y, 0, 0};
    std::array<glm::vec2, SPRITE_COUNT> direction{};
    for (auto &i : direction) {
        static float num = 0;
        i = glm::rotate(glm::mat4(1.f), glm::radians(num), glm::vec3(0, 0, 1)) * (move1 -
                                                                                  move2);
        num += 10;
    }
    while (!win.run()) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        if (currentFrame - lastFrame >= 1.f / 60) {
        lastFrame = currentFrame;
        glfwSwapBuffers(win.getHNDL());
        glClear(GL_COLOR_BUFFER_BIT);
        //std::cout << 1 / deltaTime << std::endl;

        //std::cout << direction[0] << " / " << direction[1] << std::endl;
        for (int i = 0; i < SPRITE_COUNT; ++i) {
            glm::vec2 pos = ball[i].getPos();

            processBoundryCollision(direction[i], pos);
            for (int j = 0; j < SPRITE_COUNT; ++j)
                if (i == j)
                    continue;
                else
                    processBallCollision(pos, ball[j].getPos(), direction[i], direction[j], deltaTime);

            ball[i].stepX(direction[i][0] * deltaTime);
            ball[i].stepY(direction[i][1] * deltaTime);

            ball[i].update();
            ball[i].render();
            glfwPollEvents();
        }
        }
    }

    return 0;
}