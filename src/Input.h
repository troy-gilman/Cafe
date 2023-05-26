#ifndef INPUT_H
#define INPUT_H

#include "util/Types.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Input {

    // GLOBALS
    static bool _keys[GLFW_KEY_LAST];
    static bool _mouseButtons[GLFW_MOUSE_BUTTON_LAST];
    static f64 _mouseX;
    static f64 _mouseY;
    static f64 _scrollX;
    static f64 _scrollY;

    // CALLBACKS
    static void keyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
    static void cursorPositionCallback(GLFWwindow* window, f64 xPos, f64 yPos);
    static void mouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
    static void scrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset);

    enum KeyState {
        RELEASED,
        PRESSED,
        DOWN,
        UP
    };

    struct InputState {
        KeyState keys[GLFW_KEY_LAST];
        bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];
        f64 mouseX;
        f64 mouseY;
        f64 scrollX;
        f64 scrollY;
        f64 oldMouseX;
        f64 oldMouseY;
        f64 oldScrollX;
        f64 oldScrollY;
    };

    void initInputState(InputState& state, GLFWwindow* glfwWindow);
    void updateInputState(InputState& state);
}

#endif // INPUT_H