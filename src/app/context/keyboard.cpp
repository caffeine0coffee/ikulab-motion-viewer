#include "keyboard.hpp"

#include "GLFW/glfw3.h"

// NOLINTBEGIN(*-easily-swappable-parameters)
/**
 * @brief Update keyboard state from GLFW KayCallback.
 *
 * 'KeyCallback' means the function passed to GlfwSetKeyCallback().
 *
 * @param key The 'key' argument expected in KeyCallback
 * @param scanCode The 'scancode' argument expected in KeyCallback
 * @param action The 'action' argument expected in KeyCallback
 * @param mods The 'mods' argument expected in KeyCallback
 */
void Keyboard::UpdateFromKeyCallback(const int key,
                                     [[maybe_unused]] int scanCode,
                                     const int action,
                                     [[maybe_unused]] int mods) {
    switch (key) {
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
            set_ctrl(action != GLFW_RELEASE);
            break;
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:
            set_alt(action != GLFW_RELEASE);
            break;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            set_shift(action != GLFW_RELEASE);
            break;
        default:
            break;
    }
}
// NOLINTEND(*-easily-swappable-parameters)
