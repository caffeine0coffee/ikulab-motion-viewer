#pragma once

#include <atomic>

class Keyboard {
  public:
    void UpdateFromKeyCallback(int key, int scanCode, int action, int mods);

    [[nodiscard]] bool ctrl() const { return ctrl_; }
    [[nodiscard]] bool alt() const { return alt_; }
    [[nodiscard]] bool shift() const { return shift_; }

  private:
    std::atomic<bool> ctrl_ = false;
    std::atomic<bool> alt_ = false;
    std::atomic<bool> shift_ = false;
};
