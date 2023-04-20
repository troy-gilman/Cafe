#ifndef INPUT_H
#define INPUT_H

#include "util/Types.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Input {
private:
    static Input* instance;

    bool keys[GLFW_KEY_LAST];
    bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];
    f64 mouseX;
    f64 mouseY;
    f64 scrollX;
    f64 scrollY;
    f64 oldMouseX;
    f64 oldMouseY;
    f64 oldScrollX;
    f64 oldScrollY;

    Input();

    static void keyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
    static void cursorPositionCallback(GLFWwindow* window, f64 xPos, f64 yPos);
    static void mouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
    static void scrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset);

public:
    static Input* getInstance();
    void update();
    f64 getMouseX() const;
    f64 getMouseY() const;
    f64 getScrollX() const;
    f64 getScrollY() const;
    f64 getMouseDeltaX() const;
    f64 getMouseDeltaY() const;
    f64 getScrollDeltaX() const;
    f64 getScrollDeltaY() const;
    bool isKeyPressed(i32 key) const;
    bool isMouseButtonPressed(i32 button) const;
};

#endif // INPUT_H