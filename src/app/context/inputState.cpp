//
// Created by caffeine on 25/03/02.
//

#include "inputState.hpp"

#include <memory>
#include <mutex>

std::shared_ptr<InputState> InputState::instance_ = nullptr;
std::mutex InputState::instance_mutex_;

InputState::InputState()
    : keyboard_(std::make_shared<Keyboard>()),
      mouse_(std::make_shared<Mouse>()) {}

std::shared_ptr<InputState> InputState::GetInstance() {
    std::lock_guard<std::mutex> const lock(instance_mutex_);
    if (instance_ == nullptr) {
        instance_ = std::make_shared<InputState>();
    }

    return instance_;
}

void InputState::cursorPositionCallback(GLFWwindow* window, double xPos,
                                        double yPos) {
    const auto input_state = GetInstance();

    input_state->mouse_->UpdateFromCursorPositionCallback(xPos, yPos);
}

void InputState::mouseButtonCallback(GLFWwindow* window, int button, int action,
                                     int mods) {
    const auto input_state = GetInstance();

    input_state->mouse_->UpdateFromMouseButtonCallback(button, action, mods);
}

void InputState::scrollCallback(GLFWwindow* window, double xOffset,
                                double yOffset) {
    const auto input_state = GetInstance();

    input_state->mouse_->UpdateFromScrollCallback(xOffset, yOffset);
}

void InputState::keyCallback(GLFWwindow* window, int key, int scanCode,
                             int action, int mods) {
    const auto input_state = GetInstance();

    input_state->keyboard_->UpdateFromKeyCallback(key, scanCode, action, mods);
}
