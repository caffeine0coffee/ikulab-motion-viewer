#pragma once

#include <atomic>

class Mouse {
  public:
    void Reset();
    void InitDragPosition();

    void UpdateFromCursorPositionCallback(double x_pos, double y_pos);
    void UpdateFromMouseButtonCallback(int button, int action, int mods);
    void UpdateFromScrollCallback(double x_offset, double y_offset);

    [[nodiscard]] bool left_button() const { return left_button_; }
    [[nodiscard]] bool right_button() const { return right_button_; }
    [[nodiscard]] bool middle_button() const { return middle_button_; }

    [[nodiscard]] double drag_start_x() const { return drag_start_x_; }
    [[nodiscard]] double drag_start_y() const { return drag_start_y_; }

    [[nodiscard]] double current_x() const { return current_x_; }
    [[nodiscard]] double current_y() const { return current_y_; }

    [[nodiscard]] double delta_x() const { return delta_x_; }
    [[nodiscard]] double delta_y() const { return delta_y_; }

    [[nodiscard]] double scroll_offset_x() const { return scroll_offset_x_; }
    [[nodiscard]] double scroll_offset_y() const { return scroll_offset_y_; }

  private:
    std::atomic<bool> left_button_ = false;
    std::atomic<bool> right_button_ = false;
    std::atomic<bool> middle_button_ = false;

    std::atomic<double> drag_start_x_ = 0.0;
    std::atomic<double> drag_start_y_ = 0.0;
    std::atomic<double> drag_end_x_ = 0.0;
    std::atomic<double> drag_end_y_ = 0.0;

    std::atomic<double> current_x_ = 0.0;
    std::atomic<double> current_y_ = 0.0;

    std::atomic<double> delta_x_ = 0.0;
    std::atomic<double> delta_y_ = 0.0;

    std::atomic<double> scroll_offset_x_ = 0.0;
    std::atomic<double> scroll_offset_y_ = 0.0;
};
