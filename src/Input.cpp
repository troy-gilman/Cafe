#include "Input.h"

Input* Input::instance = nullptr;

Input::Input() {
    GLFWwindow* glfwWindow = glfwGetCurrentContext();
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetCursorPosCallback(glfwWindow, cursorPositionCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
    glfwSetScrollCallback(glfwWindow, scrollCallback);

    for (i32 i = 0; i < GLFW_KEY_LAST; i++) {
        keys[i] = false;
    }

    for (i32 i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
        mouseButtons[i] = false;
    }

    mouseX = 0.0;
    mouseY = 0.0;
    scrollX = 0.0;
    scrollY = 0.0;
    oldMouseX = 0.0;
    oldMouseY = 0.0;
    oldScrollX = 0.0;
    oldScrollY = 0.0;
}

void Input::keyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    if (key >= 0 && key < GLFW_KEY_LAST) {
        if (action == GLFW_PRESS) {
            instance->keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            instance->keys[key] = false;
        }
    }
}

void Input::cursorPositionCallback(GLFWwindow* window, f64 xPos, f64 yPos) {
    instance->mouseX = xPos;
    instance->mouseY = yPos;
}

void Input::mouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
    if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
        if (action == GLFW_PRESS) {
            instance->mouseButtons[button] = true;
        } else if (action == GLFW_RELEASE) {
            instance->mouseButtons[button] = false;
        }
    }
}

void Input::scrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset) {
    instance->scrollX += xOffset;
    instance->scrollY += yOffset;
}

Input* Input::getInstance() {
    if (instance == nullptr) {
        instance = new Input();
    }
    return instance;
}

void Input::update() {
    oldMouseX = mouseX;
    oldMouseY = mouseY;
    oldScrollX = scrollX;
    oldScrollY = scrollY;
}

f64 Input::getMouseX() const {
    return mouseX;
}

f64 Input::getMouseY() const {
    return mouseY;
}

f64 Input::getScrollX() const {
    return scrollX;
}

f64 Input::getScrollY() const {
    return scrollY;
}

f64 Input::getMouseDeltaX() const {
    return mouseX - oldMouseX;
}

f64 Input::getMouseDeltaY() const {
    return mouseY - oldMouseY;
}

f64 Input::getScrollDeltaX() const {
    return scrollX - oldScrollX;
}

f64 Input::getScrollDeltaY() const {
    return scrollY - oldScrollY;
}

bool Input::isKeyPressed(i32 key) const {
    return keys[key];
}

bool Input::isMouseButtonPressed(i32 button) const {
    return mouseButtons[button];
}