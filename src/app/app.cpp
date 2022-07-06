#include <iostream>
#include <memory>
#include <array>
#include <algorithm>

#include <glm/glm.hpp>

#include "app.hpp"
#include "base.hpp"

#include "animator.hpp"

#include "./shape/bone/stickTetrahedronBone.hpp"
#include "./shape/floor/gridFloor.hpp"
#include "./shape/floor/filledFloor.hpp"

#include "./shape/debug/directionDebugObject.hpp"

void App::init() {
	base = std::make_unique<Base>();

	anim = std::make_shared<Animator>();
	anim->initFromBVH("./models/swing.bvh");
	// anim->showSkeltonInfo();
	// anim->showMotionInfo();

	camera = std::make_unique<Camera>();
	keyboard = std::make_unique<Keyboard>();
	mouse = std::make_unique<Mouse>();

	modelUbo = std::make_unique<ModelMatUBO>();
	sceneUbo = std::make_unique<SceneMatUBO>();

	createShapes();
	registerShapes();
	registerInputEvents();

	base->setAnimator(anim);
	base->initVulkan();
	base->initImGui();
}

void App::createShapes() {
	// staging shapes vector
	std::vector<std::unique_ptr<Shape>> tmpShapes;
	// staging bones array
	std::array<std::unique_ptr<Shape>, NUM_OF_JOINT_ID> tmpBones = anim->generateBones();
	// move only existing bones to shapes vector
	for (auto& elm : tmpBones) {
		if (elm == nullptr) {
			continue;
		}
		tmpShapes.push_back(std::move(elm));
	}
	tmpShapes.push_back(std::move(std::make_unique<GridFloor>(
		1000.0, 1000.0,
		1, 10, 10,
		glm::vec3(0.2, 0.9, 0.2),
		FLOOR_ID
	)));
	tmpShapes.push_back(std::move(std::make_unique<DirectionDebugObject>(
		40.0, DEBUG_OBJECT_ID
	)));

	// set base index
	uint32_t baseIndex = 0;
	for (int i = 0; i < anim->getNumOfJoints() + NUM_OF_ID_OTHER_THAN_JOINTS; i++) {
		tmpShapes[i]->setBaseIndex(baseIndex);
		baseIndex += static_cast<uint32_t>(tmpShapes[i]->getVertices().size());
	}

	// move to shapes vector
	shapes.assign(
		std::make_move_iterator(tmpShapes.begin()),
		std::make_move_iterator(tmpShapes.end())
	);
}

void App::registerShapes() {
	for (const auto& shape : shapes) {
		base->addVertices(shape->getVertices());
		base->addIndices(shape->getIndices());
	}
}

void App::updateUbo() {
	// Model Mat ----------
	std::array<glm::mat4, NUM_OF_JOINT_ID> modelMats
		= anim->generateModelMatrices(base->getSecondsFromStart());

	// bone transformation
	for (int i = 0; i < anim->getNumOfJoints(); i++) {
		modelUbo->model[i] = modelMats[i];
	}

	// debug
#ifdef NODEBUG
	modelUbo->model[DEBUG_OBJECT_ID] = glm::mat4(0.0);
#endif

	// global scaling
	for (auto& m : modelUbo->model) {
		m = glm::scale(glm::mat4(1.0), glm::vec3(0.01)) * m;
	}

	base->updateModelMatUniformBuffer(*modelUbo);

	// Scene Mat ----------
	VkExtent2D extent = base->getSwapChainExtent();
	sceneUbo->view = camera->generateViewMat();
	sceneUbo->proj = glm::perspective(
		glm::radians(45.0f),
		extent.width / (float)extent.height,
		0.01f,
		1000.0f
	);

	// デフォルトでは 左手系 Z-down になっている
	// この式によって 右手系 Z-up に変換する
	sceneUbo->proj[1][1] *= -1;
	
	base->updateSceneMatUniformBuffer(*sceneUbo);
}

void App::run() {
	while (!base->windowShouldClose()) {
		base->vSync();
		base->updateClock();

		base->pollWindowEvent();
		base->updateCamera();
		updateUbo();

		base->drawImGuiFrame();
		base->drawFrame();
	}
}

void App::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    App* appPtr = static_cast<App*>(glfwGetWindowUserPointer(window));

    appPtr->mouse->deltaX = xPos - appPtr->mouse->currentX;
    appPtr->mouse->deltaY = yPos - appPtr->mouse->currentY;

    appPtr->mouse->currentX = xPos;
    appPtr->mouse->currentY = yPos;

    // record drag end position
    if (appPtr->mouse->leftButton) {
        appPtr->mouse->dragEndX = xPos;
        appPtr->mouse->dragEndY = yPos;
    }
}

void App::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    App* appPtr = static_cast<App*>(glfwGetWindowUserPointer(window));
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        appPtr->mouse->leftButton = (action == GLFW_PRESS);
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        appPtr->mouse->rightButton = (action == GLFW_PRESS);
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        appPtr->mouse->middleButton = (action == GLFW_PRESS);
        break;
    default:
        break;
    }

    // init drag position
    if (appPtr->mouse->leftButton) {
        appPtr->mouse->dragStartX = appPtr->mouse->currentX;
        appPtr->mouse->dragStartY = appPtr->mouse->currentY;
        appPtr->mouse->dragEndX = appPtr->mouse->currentX;
        appPtr->mouse->dragEndY = appPtr->mouse->currentY;
    }
}

void App::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    App* appPtr = static_cast<App*>(glfwGetWindowUserPointer(window));
    appPtr->mouse->scrollOffsetX = xOffset;
    appPtr->mouse->scrollOffsetY = yOffset;
}

void App::keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    App* appPtr = static_cast<App*>(glfwGetWindowUserPointer(window));

    switch (key) {
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        appPtr->keyboard->ctrl = (action == GLFW_PRESS);
        break;
    case GLFW_KEY_LEFT_ALT:
    case GLFW_KEY_RIGHT_ALT:
        appPtr->keyboard->alt = (action == GLFW_PRESS);
        break;
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
        appPtr->keyboard->shift = (action == GLFW_PRESS);
        break;
    default:
        break;
    }
}

void App::registerInputEvents() {
	auto window = base->getGlfwWindow();
    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
}
