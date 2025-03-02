#include "app.hpp"

void App::cursorPositionCallback(GLFWwindow *window, double xPos, double yPos) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    app->mouse->UpdateFromCursorPositionCallback(xPos, yPos);
}

void App::mouseButtonCallback(GLFWwindow *window, int button, int action,
                              int mods) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    app->mouse->UpdateFromMouseButtonCallback(button, action, mods);
}

void App::scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    app->mouse->UpdateFromScrollCallback(xOffset, yOffset);
}

void App::keyCallback(GLFWwindow *window, int key, int scanCode, int action,
                      int mods) {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    app->keyboard->UpdateFromKeyCallback(key, scanCode, action, mods);
}
