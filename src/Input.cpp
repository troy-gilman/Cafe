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

void Input::initInputState(Input::InputState *state, GLFWwindow *glfwWindow) {
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetCursorPosCallback(glfwWindow, cursorPositionCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
    glfwSetScrollCallback(glfwWindow, scrollCallback);

    for (i32 i = 0; i < GLFW_KEY_LAST; i++) {
        state->keys[i] = false;
    }

    for (i32 i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
        state->mouseButtons[i] = false;
    }

    state->mouseX = 0.0;
    state->mouseY = 0.0;
    state->scrollX = 0.0;
    state->scrollY = 0.0;
    state->oldMouseX = 0.0;
    state->oldMouseY = 0.0;
    state->oldScrollX = 0.0;
    state->oldScrollY = 0.0;
}

void Input::updateInputState(Input::InputState *state) {
    state->oldMouseX = state->mouseX;
    state->oldMouseY = state->mouseY;
    state->oldScrollX = state->scrollX;
    state->oldScrollY = state->scrollY;

    state->mouseX = _mouseX;
    state->mouseY = _mouseY;
    state->scrollX = _scrollX;
    state->scrollY = _scrollY;

    memcpy(state->keys, _keys, sizeof(bool) * GLFW_KEY_LAST);
    memcpy(state->mouseButtons, _mouseButtons, sizeof(bool) * GLFW_MOUSE_BUTTON_LAST);
}