#pragma once

#include <memory>
#include <mutex>

#include "GLFW/glfw3.h"

#include "keyboard.hpp"
#include "mouse.hpp"

class InputState {
  public:
    InputState();

    static std::shared_ptr<InputState> GetInstance();

    [[nodiscard]] std::shared_ptr<Keyboard> keyboard() const {
        return keyboard_;
    }
    [[nodiscard]] std::shared_ptr<Mouse> mouse() const {
        return mouse_;
    }

    static void cursorPositionCallback(GLFWwindow* window, double xPos,
                                       double yPos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action,
                                    int mods);
    static void scrollCallback(GLFWwindow* window, double xOffset,
                               double yOffset);
    static void keyCallback(GLFWwindow* window, int key, int scanCode,
                            int action, int mods);

  private:
    static std::shared_ptr<InputState> instance_;
    static std::mutex instance_mutex_;

    std::shared_ptr<Keyboard> keyboard_;
    std::shared_ptr<Mouse> mouse_;
};
