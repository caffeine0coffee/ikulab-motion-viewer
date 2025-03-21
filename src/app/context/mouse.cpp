#include "mouse.hpp"

#include "GLFW/glfw3.h"

void Mouse::Reset() {
    scroll_offset_x_ = 0;
    scroll_offset_y_ = 0;
    delta_x_ = 0;
    delta_y_ = 0;
}

/**
 * @brief Update mouse state from GLFW CursorPositionCallback.
 *
 * 'CursorPositionCallback' means the function passed to GlfwSetKeyCallback().
 *
 * @param x_pos The 'x position' argument expected in CursorPositionCallback
 * @param y_pos The 'y position' argument expected in CursorPositionCallback
 */
void Mouse::UpdateFromCursorPositionCallback(const double x_pos,
                                             const double y_pos) {
    delta_x_ = x_pos - current_x();
    delta_y_ = y_pos - current_y();
    current_x_ = x_pos;
    current_y_ = y_pos;
}

// NOLINTBEGIN(*-easily-swappable-parameters)
/**
 * @brief Update mouse state from GLFW MouseButtonCallback.
 *
 * 'MouseButtonCallback' means the function passed to GlfwSetKeyCallback().
 *
 * @param button The 'button' argument expected in CursorPositionCallback
 * @param action The 'action' argument expected in CursorPositionCallback
 * @param mods The 'mods' argument expected in CursorPositionCallback
 */
void Mouse::UpdateFromMouseButtonCallback(const int button, const int action,
                                          [[maybe_unused]] const int mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            left_button_ = (action == GLFW_PRESS);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            right_button_ = (action == GLFW_PRESS);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            middle_button_ = (action == GLFW_PRESS);
            break;
        default:
            break;
    }

    // init drag position
    if (left_button()) {
        InitDragPosition();
    }
}
// NOLINTEND(*-easily-swappable-parameters)

/**
 * @brief Update mouse state from GLFW ScrollCallback.
 *
 * 'ScrollCallback' means the function passed to GlfwSetKeyCallback().
 *
 * @param x_offset The 'x offset' argument expected in ScrollCallback
 * @param y_offset The 'y offset' argument expected in ScrollCallback
 */
void Mouse::UpdateFromScrollCallback(const double x_offset,
                                     const double y_offset) {
    scroll_offset_x_ = x_offset;
    scroll_offset_y_ = y_offset;
}

void Mouse::InitDragPosition() {
    drag_start_x_ = current_x();
    drag_start_y_ = current_y();
}
