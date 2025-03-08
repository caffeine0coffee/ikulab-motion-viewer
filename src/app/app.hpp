#pragma once

#include <memory>

#include <ikura/ikura.hpp>

#include "context/inputState.hpp"
#include "context/camera.hpp"
#include "context/keyboard.hpp"
#include "context/mouse.hpp"
#include "context/ui.hpp"
#include "motionUtil/animator.hpp"

class App {
    // Variables ==========
    // Constants ----------
    const int NUM_OF_GROUPS_OTHER_THAN_JOINTS = 2;
    const ikura::GroupID AXIS_OBJ_GROUP_ID = ikura::NUM_OF_MODEL_MATRIX - 3;
    const ikura::GroupID FLOOR_GROUP_ID = ikura::NUM_OF_MODEL_MATRIX - 2;

    // ikura objects ----------
    std::unique_ptr<ikura::AppEngine> appEngine;
    std::shared_ptr<ikura::RenderEngine> renderEngine;

    std::shared_ptr<ikura::GlfwNativeWindow> mainWindow;
    std::shared_ptr<ikura::BasicRenderTarget> mainRenderTarget;
    std::shared_ptr<ikura::BasicRenderContent> mainRenderContent;

    std::shared_ptr<ikura::ImGuiVirtualWindow> imGuiVirtualWindow;

    std::shared_ptr<ikura::BasicRenderComponentProvider>
        basicRenderComponentProvider;

    // Contexts ----------
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Keyboard> keyboard;
    std::shared_ptr<Mouse> mouse;
    std::shared_ptr<UI> ui;
    std::shared_ptr<InputState> input_state_;

    // Flags ----------
    bool modelLoaded = false;

    // Others ----------
    std::shared_ptr<Animator> animator;

    // Functions ==========
    // Init ----------
    void initIkura();
    void setShapes(const char *filePath);
    void initContexts();
    void setGlfwWindowEvents(GLFWwindow *window);

	// select file ----------
    void selectFileAndInitShapes();
    void selectFileAndExportLoopRange();

    // Update ----------
    void updateMatrices();

    // UI ----------
    void updateUI();
    void updateMainMenu();
    void updateAnimationControlWindow();
    void updateDebugWindow();

  public:
    App();
    void run();
};
