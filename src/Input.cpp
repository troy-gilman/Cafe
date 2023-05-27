#include "Input.h"
#include <cstring>

void Input::keyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    if (key >= 0 && key < GLFW_KEY_LAST) {
        if (action == GLFW_PRESS) {
            _keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            _keys[key] = false;
        }
    }
}

void Input::cursorPositionCallback(GLFWwindow* window, f64 xPos, f64 yPos) {
    _mouseX = xPos;
    _mouseY = yPos;
}

void Input::mouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
    if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
        if (action == GLFW_PRESS) {
            _mouseButtons[button] = true;
        } else if (action == GLFW_RELEASE) {
            _mouseButtons[button] = false;
        }
    }
}

void Input::scrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset) {
    _scrollX += xOffset;
    _scrollY += yOffset;
}

void Input::initInputData(InputData &input, GLFWwindow *glfwWindow) {
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetCursorPosCallback(glfwWindow, cursorPositionCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
    glfwSetScrollCallback(glfwWindow, scrollCallback);

    for (i32 i = 0; i < GLFW_KEY_LAST; i++) {
        input.keys[i] = KeyState::RELEASED;
    }

    for (i32 i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
        input.mouseButtons[i] = false;
    }

    input.mouseX = 0.0;
    input.mouseY = 0.0;
    input.scrollX = 0.0;
    input.scrollY = 0.0;
    input.oldMouseX = 0.0;
    input.oldMouseY = 0.0;
    input.oldScrollX = 0.0;
    input.oldScrollY = 0.0;
}

void Input::updateInputData(InputData& input) {
    input.oldMouseX = input.mouseX;
    input.oldMouseY = input.mouseY;
    input.oldScrollX = input.scrollX;
    input.oldScrollY = input.scrollY;

    input.mouseX = _mouseX;
    input.mouseY = _mouseY;
    input.scrollX = _scrollX;
    input.scrollY = _scrollY;

    for (i32 i = 0; i < GLFW_KEY_LAST; i++) {
        if (_keys[i]) {
            if (input.keys[i] == KeyState::RELEASED || input.keys[i] == KeyState::UP) {
                input.keys[i] = KeyState::DOWN;
            } else {
                input.keys[i] = KeyState::PRESSED;
            }
        } else {
            if (input.keys[i] == KeyState::PRESSED || input.keys[i] == KeyState::DOWN) {
                input.keys[i] = KeyState::UP;
            } else {
                input.keys[i] = KeyState::RELEASED;
            }
        }
    }

    memcpy(input.mouseButtons, _mouseButtons, sizeof(bool) * GLFW_MOUSE_BUTTON_LAST);
}

bool Input::isKeyDownOrPressed(KeyState keyState) {
    return keyState == KeyState::DOWN || keyState == KeyState::PRESSED;
}