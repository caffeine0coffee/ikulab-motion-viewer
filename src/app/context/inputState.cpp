//
// Created by caffeine on 25/03/02.
//

#include "InputState.hpp"
InputState::InputState()
    : keyboard_(std::make_shared<Keyboard>()),
      mouse_(std::make_shared<Mouse>()) {}


void InputState::cursorPositionCallback(GLFWwindow* window, double xPos,
                                        double yPos) {
    const auto* input_state =
        static_cast<InputState*>(glfwGetWindowUserPointer(window));

    input_state->mouse_->UpdateFromCursorPositionCallback(xPos, yPos);
}

void InputState::mouseButtonCallback(GLFWwindow* window, int button, int action,
                                     int mods) {
    const auto* input_state =
        static_cast<InputState*>(glfwGetWindowUserPointer(window));

    input_state->mouse_->UpdateFromMouseButtonCallback(button, action, mods);
}

void InputState::scrollCallback(GLFWwindow* window, double xOffset,
                                double yOffset) {
    const auto* input_state =
        static_cast<InputState*>(glfwGetWindowUserPointer(window));

    input_state->mouse_->UpdateFromScrollCallback(xOffset, yOffset);
}

void InputState::keyCallback(GLFWwindow* window, int key, int scanCode,
                             int action, int mods) {
    const auto* input_state =
        static_cast<InputState*>(glfwGetWindowUserPointer(window));

    input_state->keyboard_->UpdateFromKeyCallback(key, scanCode, action, mods);
}
