﻿#include "./app.hpp"

#include <algorithm>
#include <cmath>

#include <imgui.h>

#include "./motionUtil/bvhExporter.hpp"

#define MAIN_CONTROL_BUTTON_SIZE_UNIT 40

#define ANIM_WINDOW_HEIGHT_NORMAL 250
#define ANIM_WINDOW_HEIGHT_EDIT 400

void App::updateUI() {
    imGuiVirtualWindow->setCurrentImGuiContext();
    imGuiVirtualWindow->newFrame();

    updateMainMenu();

    if (ui->animationControlWindow.show) {
        updateAnimationControlWindow();
    }
    if (ui->debugWindow.show) {
        updateDebugWindow();
    }
    if (ui->showImGuiDemoWindow) {
        ImGui::ShowDemoWindow();
    }

    ImGui::Render();
}

void App::updateMainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(u8"ファイル")) {
            if (ImGui::MenuItem(u8"BVHファイルを開く")) {
                selectFileAndInitShapes();
            }
            if (ImGui::MenuItem(u8"ループ範囲をエクスポート")) {
                selectFileAndExportLoopRange();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(u8"ウィンドウ")) {
            ImGui::Checkbox(u8"デバッグウィンドウ", &ui->debugWindow.show);
            ImGui::Checkbox(u8"アニメーションコントロールウィンドウ",
                            &ui->animationControlWindow.show);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(u8"表示")) {
            ImGui::Checkbox(u8"軸オブジェクト", &ui->showAxisObject);
            ImGui::Checkbox(u8"床", &ui->showFloor);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(u8"設定")) {
            // Rotation Order --------------------
            if (!modelLoaded) {
                ImGui::BeginDisabled();
            }
            ImGui::Text(u8"回転順序");
            ImGui::SameLine();
            ImGui::PushItemWidth(80);
            auto oldRotationOrderIndex = ui->config.rotationOrderIndex;
            ImGui::Combo("##rotation_order", &ui->config.rotationOrderIndex,
                         ui->config.rotationOrderComboItems,
                         IM_ARRAYSIZE(ui->config.rotationOrderComboItems));
            // if order changed, update Motion's RotationAxisEnum order
            if (oldRotationOrderIndex != ui->config.rotationOrderIndex) {
                animator->setRotationOrder(convertStrToRotationOrder(
                    ui->config.rotationOrderComboItems
                        [ui->config.rotationOrderIndex]));
            }

            if (!modelLoaded) {
                ImGui::EndDisabled();
            }

            // Export Option --------------------
            ImGui::Checkbox(u8"全てのPositionチャンネルをエクスポート",
                            &ui->config.exportAllPositionChannel);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

// ----------------------------------------
// Animation Control Window
// ----------------------------------------

void initAnimationControlWindowSize(
    std::shared_ptr<ikura::GlfwNativeWindow> mainWindow,
    UI::AnimationControlWindow &ctx);
void updateAnimationControlWindowModeSwitcher(
    UI::AnimationControlWindow &ctx, std::shared_ptr<Animator> animator);
void updateAnimationControlWindowSeekbar(UI::AnimationControlWindow &ctx,
                                         bool &modelLoaded,
                                         std::shared_ptr<Animator> animator);
void updateAnimationControlWindowMainController(
    std::shared_ptr<Animator> animator);
void updateAnimationControlWindowSpeedController(
    std::shared_ptr<Animator> animator);
void updateAnimationControlWindowEditor(bool &modelLoaded,
                                        std::shared_ptr<Animator> animator);

void App::updateAnimationControlWindow() {
    initAnimationControlWindowSize(mainWindow, ui->animationControlWindow);

    ImGui::Begin(u8"アニメーションコントロール");

    if (!modelLoaded) {
        ImGui::BeginDisabled();
    }

    updateAnimationControlWindowModeSwitcher(ui->animationControlWindow,
                                             animator);
    UI::makePadding(20);

    updateAnimationControlWindowSeekbar(ui->animationControlWindow, modelLoaded,
                                        animator);

    if (ui->animationControlWindow.modeIndex ==
        UI::AnimationControlWindow::MODE_INDEX_EDIT) {
        UI::makePadding(20);
        updateAnimationControlWindowEditor(modelLoaded, animator);
        UI::makePadding(20);
    }

    updateAnimationControlWindowMainController(animator);
    updateAnimationControlWindowSpeedController(animator);

    if (!modelLoaded) {
        ImGui::EndDisabled();
    }
    ImGui::End();
}

void initAnimationControlWindowSize(
    std::shared_ptr<ikura::GlfwNativeWindow> mainWindow,
    UI::AnimationControlWindow &ctx) {

    if (!ctx.windowInitialized) {
        ImVec2 newWindowSize;
        ImVec2 newWindowPos;
        auto modeIndex = ctx.modeIndex;

        // Normal mode
        if (modeIndex == ctx.MODE_INDEX_NORMAL) {
            newWindowSize = ImVec2(800, ANIM_WINDOW_HEIGHT_NORMAL);
            newWindowPos = ImVec2((mainWindow->getWidth() - 800) / 2,
                                  mainWindow->getHeight() -
                                      ANIM_WINDOW_HEIGHT_NORMAL - 10);
        }
        // Edit mode
        else if (modeIndex == ctx.MODE_INDEX_EDIT) {
            newWindowSize = ImVec2(800, ANIM_WINDOW_HEIGHT_EDIT);
            newWindowPos =
                ImVec2((mainWindow->getWidth() - 800) / 2,
                       mainWindow->getHeight() - ANIM_WINDOW_HEIGHT_EDIT - 10);
        }

        ImGui::SetNextWindowSize(newWindowSize);
        ImGui::SetNextWindowPos(newWindowPos);
        ctx.windowInitialized = true;
    }
}

void updateAnimationControlWindowModeSwitcher(
    UI::AnimationControlWindow &ctx, std::shared_ptr<Animator> animator) {
    int oldModeIndex = ctx.modeIndex;

    ImGui::Text("Mode");
    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    ImGui::Combo("##mode_select", &ctx.modeIndex, ctx.MODE_ITEMS.data(),
                 ctx.MODE_ITEMS.size());
    ImGui::PopItemWidth();

    // if changed, window size & position will be initialized
    if (oldModeIndex != ctx.modeIndex) {
        ctx.windowInitialized = false;
        animator->setLoopEnabled(ctx.modeIndex ==
                                 UI::AnimationControlWindow::MODE_INDEX_EDIT);
    }
}

void updateAnimationControlWindowSeekbar(UI::AnimationControlWindow &ctx,
                                         bool &modelLoaded,
                                         std::shared_ptr<Animator> animator) {
    auto maxFrameNum = animator->getNumOfFrames();
    auto currentFrameNum = animator->getCurrentFrameIndex() + 1;

    if (modelLoaded) {
        ImGui::Text("Frame: %d / %d", currentFrameNum, maxFrameNum);
    } else {
        ImGui::Text("Frame: -- / --");
    }

    ImGui::PushItemWidth(-1);
    if (modelLoaded) {
        int seekBarValue = currentFrameNum;
        int oldSeekBarValue = seekBarValue;

        ImGui::SliderInt("##seek_bar", &seekBarValue, 1,
                         animator->getNumOfFrames());
        ctx.isSeekBarDragging = ImGui::IsItemActive();

        if (oldSeekBarValue != seekBarValue) {
            animator->seekAnimation(seekBarValue - 1);
        }
    } else {
        int unused = 0;
        ImGui::SliderInt("##seek_bar", &unused, 0, 0);
    }
    ImGui::PopItemWidth();
}

void updateAnimationControlWindowMainController(
    std::shared_ptr<Animator> animator) {
    // Align
    float space = ImGui::GetStyle().ItemSpacing.x;
    float width = MAIN_CONTROL_BUTTON_SIZE_UNIT * 8 + space * 6;
    float avail = ImGui::GetWindowContentRegionWidth();
    float pos = (avail - width) / 2 + ImGui::GetCursorPosX();
    ImGui::SetCursorPosX(pos);

    // Jump to begin
    if (ImGui::Button("<<##jump_to_begin",
                      ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                             MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animator->seekAnimation(0);
    }
    ImGui::SameLine();

    // Prev frame
    if (ImGui::Button("-5##prev_5", ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                                           MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animator->incrementFrameIndex(-5);
    }
    ImGui::SameLine();

    if (ImGui::Button("-1##prev_1", ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                                           MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animator->incrementFrameIndex(-1);
    }
    ImGui::SameLine();

    // Play button
    const char *playButtonLabel =
        animator->isAnimationStopped() ? "Play" : "Stop";
    if (ImGui::Button(playButtonLabel, ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT * 2,
                                              MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        if (animator->isAnimationStopped()) {
            animator->resumeAnimation();
        } else {
            animator->stopAnimation();
        }
    }
    ImGui::SameLine();

    // Next frame
    if (ImGui::Button("+1##next_1", ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                                           MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animator->incrementFrameIndex(1);
    }
    ImGui::SameLine();

    if (ImGui::Button("+5##next_5", ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                                           MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animator->incrementFrameIndex(5);
    }
    ImGui::SameLine();

    // Jump to end
    if (ImGui::Button(">>##jump_to_end",
                      ImVec2(MAIN_CONTROL_BUTTON_SIZE_UNIT,
                             MAIN_CONTROL_BUTTON_SIZE_UNIT))) {
        animator->seekAnimation(animator->getNumOfFrames() - 1);
    }
}
void updateAnimationControlWindowSpeedController(
    std::shared_ptr<Animator> animator) {
    float animationSpeed = animator->getAnimationSpeed();

    ImGui::Text("Speed");

    ImGui::PushItemWidth(20);
    if (ImGui::Button("<<##speed_down")) {
        animationSpeed /= 2;
        animationSpeed = std::max({animationSpeed, MIN_ANIMATION_SPEED});
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("##speed_drag", &animationSpeed, 0.1, MIN_ANIMATION_SPEED,
                     MAX_ANIMATION_SPEED);

    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::Button(">>##speed_up")) {
        animationSpeed *= 2;
        animationSpeed = std::min({animationSpeed, MAX_ANIMATION_SPEED});
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset##speed_reset")) {
        animationSpeed = 1.0;
    }

    animator->setAnimationSpeed(animationSpeed);
}

void updateAnimationControlWindowEditor(bool &modelLoaded,
                                        std::shared_ptr<Animator> animator) {
    // *Num starts from 1 (user-friendly expression)
    // *Index starts from 0
    int newLoopStartFrameNum = animator->getLoopStartFrameIndex() + 1;
    int newLoopEndFrameNum = animator->getLoopEndFrameIndex() + 1;

    if (modelLoaded) {
        // loop start ----------
        ImGui::Text("loop start");
        ImGui::SameLine();

        ImGui::PushItemWidth(100);
        ImGui::InputInt("##editor_start_input", &newLoopStartFrameNum);
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(-1);
        ImGui::SliderInt("##editor_start", &newLoopStartFrameNum, 1,
                         animator->getNumOfFrames());
        ImGui::PopItemWidth();

        newLoopStartFrameNum =
            std::clamp(newLoopStartFrameNum, 1, newLoopEndFrameNum);

        // loop end ----------
        ImGui::Text("loop end  ");
        ImGui::SameLine();

        ImGui::PushItemWidth(100);
        ImGui::InputInt("##editor_end_input", &newLoopEndFrameNum);
        ImGui::PopItemWidth();

        ImGui::PushItemWidth(-1);
        ImGui::SliderInt("##editor_end", &newLoopEndFrameNum, 1,
                         animator->getNumOfFrames());
        ImGui::PopItemWidth();

        newLoopEndFrameNum =
            std::clamp(newLoopEndFrameNum, newLoopStartFrameNum,
                       (int)animator->getNumOfFrames());

        // update loop range ----------
        if ((newLoopStartFrameNum != animator->getLoopStartFrameIndex() + 1) ||
            (newLoopEndFrameNum != animator->getLoopEndFrameIndex() + 1)) {
            animator->updateLoopRange(newLoopStartFrameNum - 1,
                                      newLoopEndFrameNum - 1);
        }
    } else {
        ImGui::PushItemWidth(-1);

        int unused = 0;
        ImGui::SliderInt("##editor_start", &unused, 0, 0);
        ImGui::SliderInt("##editor_end", &unused, 0, 0);

        ImGui::PopItemWidth();
    }
}

// ----------------------------------------
// Debug window
// ----------------------------------------

void App::updateDebugWindow() {
    if (!ui->debugWindow.sizeInitialized) {
        ImGui::SetNextWindowSize(ImVec2(300, 500));
        ui->debugWindow.sizeInitialized = true;
    }

    ImGui::Begin(u8"デバッグ");
    ImGuiIO &io = ImGui::GetIO();

    ImGui::Checkbox(u8"ImGui DemoWindowを表示する##show_imgui_window",
                    &ui->showImGuiDemoWindow);
    ImGui::Checkbox(u8"軸オブジェクトを表示する##show_axis_object",
                    &ui->showAxisObject);
    ImGui::Checkbox(u8"床を表示する##show_floor", &ui->showFloor);
    // ImGui::Checkbox("垂直同期を有効化する##enable_vsinc", &ui->enableVsinc);

    UI::makePadding(20);

    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Joints: %d", animator->getNumOfJoints());
    ImGui::Text("Animation Time: %f", animator->getAnimationTime());

    UI::makePadding(10);

    // mouse input status
    ImGui::Text("Cursor Pos: (%.1f, %.1f)", mouse->currentX, mouse->currentY);
    ImGui::Text("DragStart: (%.1f, %.1f)", mouse->dragStartX,
                mouse->dragStartY);
    ImGui::Text("DragEnd: (%.1f, %.1f)", mouse->dragEndX, mouse->dragEndY);
    ImGui::Text("Button L/R/M: (%d / %d / %d)", mouse->leftButton,
                mouse->rightButton, mouse->middleButton);

    UI::makePadding(10);

    // camera status
    ImGui::Text("Camera rotation (degrees) H/V: (%.2f, %.2f)",
                glm::degrees(camera->hRotation),
                glm::degrees(camera->vRotation));
    ImGui::Text("Camera distance: %.2f", camera->distance);
    glm::vec3 cameraPos = camera->generatePos();
    ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y,
                cameraPos.z);
    ImGui::Text("Camera look-at position: (%.2f, %.2f, %.2f)", camera->center.x,
                camera->center.y, camera->center.z);

    UI::makePadding(10);

    // window status
    ImGui::Text("Window size: (%d, %d)", mainWindow->getWidth(),
                mainWindow->getHeight());
    ImGui::Text("IsWindowFocused: %d", ImGui::IsWindowFocused());

    ImGui::End();
}
